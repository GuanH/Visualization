#include "VoxelModel.h"
#include "Utility.h"
#include "glm/ext/matrix_transform.hpp"
#include <cctype>
#include <glm/glm.hpp>
#include <fstream>
#include <string_view>
#include <string>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

VoxelModel::VoxelModel(const std::string& infFile, const std::string& rawFile){
    std::fill(m_histogram.begin(), m_histogram.end(), 0);
    std::fstream f("assets/"+infFile);
    m_res = {0,0,0};
    m_isOpen = false;
    if(!f.is_open()){
        std::cerr<<"Failed to Open the file : "<<infFile<<'\n';
        return;
    }
    m_voxelSize = {1.0f, 1.0f, 1.0f};
    std::string buf;
    while(std::getline(f, buf)){
        auto s1 = buf.begin();
        for(;s1!=buf.end()&&std::isspace(*s1);s1++);
        auto s2=s1;
        for(;s2!=buf.end()&&!std::isspace(*s2)&&*s2!='=';s2++);
        std::string_view field(s1, s2);
        if(*s2!='='){
            for(;s2!=buf.end()&&*s2!='=';s2++);
        }
        s2++;
        for(;s2!=buf.end()&&std::isspace(*s2);s2++);
        std::string_view value(s2, buf.end());
        switch(StringHasher(field)){
            case StringHasher("resolution"):
            case StringHasher("Resolution"):{
                auto s1 = value.begin();
                auto s2 = s1;
                for(int i=2;i>=0;i--){
                    for(;s2!=value.end()&&*s2!='x'&&*s2!=':';s2++);
                    m_res[i] = std::stoi(std::string(s1, s2));
                    if(s2==value.end()) break;
                    s2++;
                    s1=s2;
                }
            }
            case StringHasher("ratio"):
            case StringHasher("voxel-size"):
            case StringHasher("VoxelSize"):{
                auto s1 = value.begin();
                auto s2 = s1;
                for(int i=0;i<3;i++){
                    for(;s2!=value.end()&&*s2!='x'&&*s2!=':';s2++);
                    m_voxelSize[i] = std::stof(std::string(s1, s2));
                    if(s2==value.end()) break;
                    s2++;
                    s1=s2;
                }
            }
        }
    }
    auto dim = glm::vec3(m_res) * m_voxelSize;
    float mxaxis = std::max({dim.x,dim.y,dim.z});
    m_voxelSize *= 20.0f / mxaxis;
    f.close();
    f.open("assets/"+rawFile, std::ios::binary | std::ios::in);
    if(!f.is_open()){
        std::cerr<<"Failed to Open the file : "<<rawFile<<'\n';
        m_res = {0,0,0};
        return;
    }
    m_data.resize(m_res.x*m_res.y*m_res.z*sizeof(unsigned char));
    f.read(reinterpret_cast<char*>(m_data.data()), m_data.size() * sizeof(unsigned char)); 
    f.close();
    std::cout<<"Voxel Count : "<<m_data.size()<<'\n';
    std::fill(m_histogram.begin(), m_histogram.end(), 0);
    for(auto x:m_data){
        m_histogram[x]+=1;
    }
    m_voxels.resize(m_res.x, m_res.y, m_res.z);
    for(int i=0;i<m_res.x-1;i++)
        for(int j=0;j<m_res.y-1;j++)
            for(int k=0;k<m_res.z-1;k++){
                int ind0 = (i    ) * m_res.y * m_res.z + (j    ) * m_res.z + (k    );
                int ind1 = (i    ) * m_res.y * m_res.z + (j    ) * m_res.z + (k + 1);
                int ind2 = (i    ) * m_res.y * m_res.z + (j + 1) * m_res.z + (k    );
                int ind3 = (i    ) * m_res.y * m_res.z + (j + 1) * m_res.z + (k + 1);
                int ind4 = (i + 1) * m_res.y * m_res.z + (j    ) * m_res.z + (k    );
                int ind5 = (i + 1) * m_res.y * m_res.z + (j    ) * m_res.z + (k + 1);
                int ind6 = (i + 1) * m_res.y * m_res.z + (j + 1) * m_res.z + (k    );
                int ind7 = (i + 1) * m_res.y * m_res.z + (j + 1) * m_res.z + (k + 1);
                int mn = std::min({m_data[ind0],m_data[ind1],m_data[ind2],m_data[ind3],m_data[ind4],m_data[ind5],m_data[ind6],m_data[ind7]});
                int mx = std::max({m_data[ind0],m_data[ind1],m_data[ind2],m_data[ind3],m_data[ind4],m_data[ind5],m_data[ind6],m_data[ind7]});
                m_voxelTable.Add(mn, mx, {i,j,k});
                m_voxels.Get(i,j,k) = m_data[ind0] / 255.0f;
            }
    m_isOpen = true;
}

VoxelModel::~VoxelModel(){
    for(auto&it:m_surfaces){
        it.FreeBuffers();
    }
    for(auto&it:m_crossSections){
        it.FreeBuffers();
    }
}

void VoxelModel::AddSurface(){
    m_orders.push_back(m_surfaces.size());
    m_surfaces.emplace_back(this);
}

void VoxelModel::DeleteSurface(int id){
    m_surfaces[id].FreeBuffers();
    m_surfaces.erase(m_surfaces.begin()+id);
    m_orders.pop_back();
    for(int i=0;i<m_orders.size();i++)m_orders[i]=i;
}

void VoxelModel::UpdateMarchingTetra(int id, const float t){
    m_surfaces[id].UpdateMarchingTetra(t);
}

void VoxelModel::UpdateMarchingTetraOpaque(int id, const float opaque){
    m_surfaces[id].UpdateMarchingTetraOpaque(opaque);
}

void VoxelModel::DrawMarchingTetra(){
    std::sort(m_orders.begin(), m_orders.end(),[&](int x,int y){
              return m_surfaces[x].GetIsoValue()>m_surfaces[y].GetIsoValue();
    });
    for(int it:m_orders){
        m_surfaces[it].DrawMarchingTetra();
    }
}

void VoxelModel::AddCrossSection(){
    m_crossSections.emplace_back(this);
}

void VoxelModel::DeleteCrossSection(int id){
    m_crossSections[id].FreeBuffers();
    m_crossSections.erase(m_crossSections.begin()+id);
}

void VoxelModel::UpdateCrossSection(int id, const float p, const int axis){
    m_crossSections[id].UpdateCrossSection(p, axis);
}

void VoxelModel::UpdateCrossSectionOpaque(int id, const float opaque){
    m_crossSections[id].UpdateOpaque(opaque);
}

void VoxelModel::DrawCrossSection(){
    for(auto&it:m_crossSections){
        it.DrawCrossSection();
    }
}
