#include "IsoSurface.h"
#include "VoxelModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
IsoSurface::IsoSurface(const VoxelModel* model)
:r_model(model){
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glGenBuffers(1,&m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void IsoSurface::FreeBuffers(){
    GLuint b[] = {m_ebo, m_vbo};
    glDeleteBuffers(2, b);
    glDeleteVertexArrays(1, &m_vao);
}

void IsoSurface::DrawTetra(const glm::vec4&a, const glm::vec4&b, const glm::vec4&c, const glm::vec4&d){
    glm::vec3 vertices[6]={
        (m_t-a.w)/(b.w-a.w)*(glm::vec3{b}-glm::vec3{a})+glm::vec3{a},
        (m_t-a.w)/(c.w-a.w)*(glm::vec3{c}-glm::vec3{a})+glm::vec3{a},
        (m_t-a.w)/(d.w-a.w)*(glm::vec3{d}-glm::vec3{a})+glm::vec3{a},
        (m_t-b.w)/(d.w-b.w)*(glm::vec3{d}-glm::vec3{b})+glm::vec3{b},
        (m_t-b.w)/(c.w-b.w)*(glm::vec3{c}-glm::vec3{b})+glm::vec3{b},
        (m_t-d.w)/(c.w-d.w)*(glm::vec3{c}-glm::vec3{d})+glm::vec3{d},
    };
    const int table[16][5]={
        {0},//0000
        {3,0,2,1},//0001
        {3,0,4,3},//0010
        {4,2,1,4,3},//0011
        {3,1,5,4},//0100
        {4,2,5,4,0},//0101
        {4,0,1,5,3},//0110
        {3,2,5,3},//0111
        {3,2,3,5},//1000
        {4,1,0,3,5},//1001
        {4,0,4,5,2},//1010
        {3,1,4,5},//1011
        {4,1,2,3,4},//1100
        {3,0,3,4},//1101
        {3,0,1,2},//1110
        {0}//1111
    };
    int ind = (a.w>m_t) | (b.w>m_t)<<1 | (c.w>m_t)<<2 | (d.w>m_t)<<3;
    auto& ent = table[ind];
    #define ADD_VERT(x) \
    do{\
        int id = m_vertexTable.GetOrInsert(x, m_vertices.size(), 1e-4);\
        m_indices.push_back(id);\
        if(id == m_vertices.size()) m_vertices.push_back({x,w});\
        else m_vertices[id].normal+=w;\
    }while(0)
    #define ADD_TRI(x,y,z) \
    do{\
        ADD_VERT(x);\
        ADD_VERT(y);\
        ADD_VERT(z);\
    }while(0)
    if(ent[0] == 3){
        auto& x = vertices[ent[1]];
        auto& y = vertices[ent[2]];
        auto& z = vertices[ent[3]];
        auto w = glm::cross(y-x,z-x);
        ADD_TRI(x,y,z);
    }
    else if(table[ind][0] == 4){
        auto& x = vertices[ent[1]];
        auto& y = vertices[ent[2]];
        auto& z = vertices[ent[3]];
        auto& u = vertices[ent[4]];
        auto w = glm::cross(y-x,z-x);
        ADD_TRI(x,y,z);
        w = glm::cross(z-x,u-x);
        ADD_TRI(x,z,u);
    }
    #undef ADD_TRI
    #undef ADD_VERT
}


void IsoSurface::UpdateMarchingTetra(const float t){
    glBindVertexArray(m_vao);
    m_t = t;
    float H = m_t * (240.0f / 360.f);
    glm::vec4 K = {1.0f, 2.0f/3.0f, 1.0f/3.0f, 3.0f};
    glm::vec3 p = glm::abs(glm::fract(glm::vec3(H,H,H) + glm::vec3(K))*6.0f-glm::vec3(K.w,K.w,K.w));
    const auto rgb = glm::mix(glm::vec3(K.x,K.x,K.x), glm::clamp(p-glm::vec3(K.x,K.x,K.x), 0.0f, 1.0f), 1.0f);
    m_color.r = rgb.r; m_color.g = rgb.g; m_color.b = rgb.b;
    m_vertices.clear();
    m_vertexTable.Clear();
    m_indices.clear();
    r_model->m_voxelTable.ForKey(t * 255.0f, [&](int i,int j,int k){
        const glm::vec3 o = r_model->m_voxelSize * glm::vec3(i,j,k);
        const glm::vec3 s = r_model->m_voxelSize * glm::vec3(i+1,j+1,k+1);
        if((i+j+k)&1){
            DrawSplitCubeA({o.x, o.y, s.z, r_model->m_voxels.Get(i  , j  , k+1)},
                           {o.x, o.y, o.z, r_model->m_voxels.Get(i  , j  , k  )},
                           {s.x, o.y, o.z, r_model->m_voxels.Get(i+1, j  , k  )},
                           {s.x, o.y, s.z, r_model->m_voxels.Get(i+1, j  , k+1)},
                           {o.x, s.y, s.z, r_model->m_voxels.Get(i  , j+1, k+1)},
                           {o.x, s.y, o.z, r_model->m_voxels.Get(i  , j+1, k  )},
                           {s.x, s.y, o.z, r_model->m_voxels.Get(i+1, j+1, k  )},
                           {s.x, s.y, s.z, r_model->m_voxels.Get(i+1, j+1, k+1)});
        }
        else{
            DrawSplitCubeB({o.x, o.y, s.z, r_model->m_voxels.Get(i  , j  , k+1)},
                           {o.x, o.y, o.z, r_model->m_voxels.Get(i  , j  , k  )},
                           {s.x, o.y, o.z, r_model->m_voxels.Get(i+1, j  , k  )},
                           {s.x, o.y, s.z, r_model->m_voxels.Get(i+1, j  , k+1)},
                           {o.x, s.y, s.z, r_model->m_voxels.Get(i  , j+1, k+1)},
                           {o.x, s.y, o.z, r_model->m_voxels.Get(i  , j+1, k  )},
                           {s.x, s.y, o.z, r_model->m_voxels.Get(i+1, j+1, k  )},
                           {s.x, s.y, s.z, r_model->m_voxels.Get(i+1, j+1, k+1)});
        }
    });
    if(m_vertices.size()){
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(m_vertices[0]), m_vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(int), m_indices.data(), GL_STATIC_DRAW);
    }
    std::cout<<"#Vertices : "<<m_vertices.size()<<"\n#Indices : "<<m_indices.size()<<'\n';
}

void IsoSurface::UpdateMarchingTetraOpaque(const float t){
    m_color.w = t;
}

void IsoSurface::DrawMarchingTetra(){
    if(m_vertices.size()){
        glBindVertexArray(m_vao);
        glUniform4fv(2,1,glm::value_ptr(m_color));
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    }
}

void IsoSurface::DrawSplitCubeA(const glm::vec4&a,const glm::vec4&b, const glm::vec4&c, const glm::vec4&d, const glm::vec4&e, const glm::vec4&f, const glm::vec4&g, const glm::vec4&h){
    DrawTetra(a,b,c,f);
    DrawTetra(h,g,f,c);
    DrawTetra(c,d,a,h);
    DrawTetra(f,e,h,a);
    DrawTetra(a,f,c,h);
}

void IsoSurface::DrawSplitCubeB(const glm::vec4&a,const glm::vec4&b, const glm::vec4&c, const glm::vec4&d, const glm::vec4&e, const glm::vec4&f, const glm::vec4&g, const glm::vec4&h){
    DrawTetra(e,f,b,g);
    DrawTetra(d,c,g,b);
    DrawTetra(b,a,e,d);
    DrawTetra(g,h,d,e);
    DrawTetra(e,b,d,g);
}
