#include "CrossSection.h"
#include "VoxelModel.h"
#include <glm/gtc/type_ptr.hpp>
CrossSection::CrossSection(const VoxelModel* model)
:r_model(model) {
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void CrossSection::FreeBuffers(){
    glDeleteTextures(1, &m_tex);
}

void CrossSection::UpdateCrossSection(const float p, const int axis){
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    if(p<0.0f || p>1.0f)return;
    switch(axis){
        case 0:{
            int i0 = p*r_model->m_res.x;
            int i1 = i0+1;
            float r = p*r_model->m_res.x - i0;
            m_img.resize(r_model->m_res.y*r_model->m_res.z);
            for(int j=0;j<r_model->m_res.y;j++){
                for(int k=0;k<r_model->m_res.z;k++){
                    int ind0 = j * r_model->m_res.z + k;
                    int ind1 = i0 * r_model->m_res.y * r_model->m_res.z + j * r_model->m_res.z + k;
                    int ind2 = i1 * r_model->m_res.y * r_model->m_res.z + j * r_model->m_res.z + k;
                    m_img[ind0]=(r*r_model->m_data[ind1]+(1.0f-r)*r_model->m_data[ind2])/255.0f;
                }
            }
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, r_model->m_res.z, r_model->m_res.y, 0, GL_RED, GL_FLOAT, m_img.data());
            m_dim = glm::vec4(p*r_model->m_res.x*r_model->m_voxelSize.x, r_model->m_res.y*r_model->m_voxelSize.y, r_model->m_res.z*r_model->m_voxelSize.z,0);
            break;
        }
        case 1:{
            int j0 = p*r_model->m_res.y;
            int j1 = j0+1;
            float r = p*r_model->m_res.y - j0;
            m_img.resize(r_model->m_res.x*r_model->m_res.z);
            for(int i=0;i<r_model->m_res.x;i++){
                for(int k=0;k<r_model->m_res.z;k++){
                    int ind0 = i*r_model->m_res.z+k;
                    int ind1 = i * r_model->m_res.y * r_model->m_res.z + j0 * r_model->m_res.z + k;
                    int ind2 = i * r_model->m_res.y * r_model->m_res.z + j1 * r_model->m_res.z + k;
                    m_img[ind0]=(r*r_model->m_data[ind1]+(1.0f-r)*r_model->m_data[ind2])/255.0f;
                }
            }
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, r_model->m_res.z, r_model->m_res.x, 0, GL_RED, GL_FLOAT, m_img.data());
            m_dim = glm::vec4(r_model->m_res.x*r_model->m_voxelSize.x, p*r_model->m_res.y*r_model->m_voxelSize.y, r_model->m_res.z*r_model->m_voxelSize.z,1);
            break;
        }
        case 2:{
            int k0 = p*r_model->m_res.z;
            int k1 = k0+1;
            float r = p*r_model->m_res.z - k0;
            m_img.resize(r_model->m_res.x*r_model->m_res.y);
            for(int i=0;i<r_model->m_res.x;i++){
                for(int j=0;j<r_model->m_res.y;j++){
                    int ind0 = i * r_model->m_res.y + j;
                    int ind1 = i * r_model->m_res.y * r_model->m_res.z + j * r_model->m_res.z + k0;
                    int ind2 = i * r_model->m_res.y * r_model->m_res.z + j * r_model->m_res.z + k1;
                    m_img[ind0]=(r*r_model->m_data[ind1]+(1.0f-r)*r_model->m_data[ind2])/255.0f;
                }
            }
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, r_model->m_res.y, r_model->m_res.x, 0, GL_RED, GL_FLOAT, m_img.data());
            m_dim = glm::vec4(r_model->m_res.x*r_model->m_voxelSize.x, r_model->m_res.y*r_model->m_voxelSize.y, p*r_model->m_res.z*r_model->m_voxelSize.z,2);
            break;
        }
    }
}

void CrossSection::UpdateOpaque(const float opaque){
    m_opaque=opaque;
}

void CrossSection::DrawCrossSection(){
    glUniform4fv(3, 1, glm::value_ptr(m_dim));
    glUniform1f(4, m_opaque);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glDrawArrays(GL_QUADS,0,4);
}
