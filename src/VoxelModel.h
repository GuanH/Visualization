#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>
#include "Vector3.h"
#include "VoxelTable.h"
#include "IsoSurface.h"
#include "CrossSection.h"

class VoxelModel{
public:
    VoxelModel(const std::string& infFile, const std::string& rawFile);
    ~VoxelModel();
    void AddSurface();
    void DeleteSurface(int id);
    void UpdateMarchingTetra(int id, const float t);
    void UpdateMarchingTetraOpaque(int id, const float opaque);
    void DrawMarchingTetra();
    void UpdateCrossSection(int id, const float p, const int axis);
    void UpdateCrossSectionOpaque(int id, const float opaque);
    void AddCrossSection();
    void DeleteCrossSection(int id);
    void DrawCrossSection();
    bool IsOpen()const {return m_isOpen;}
    const std::array<float,256>& GetHistogram()const{return m_histogram;}
    void DrawVolume();
    glm::vec3 GetCenter() const{return glm::vec3(m_res) * m_voxelSize;};
    void UpdateTransfer(const std::array<float,256>& transfer);
private:
    GLuint m_gradTex;
    GLuint m_transferTex;
    std::vector<IsoSurface> m_surfaces;
    std::vector<CrossSection> m_crossSections;
    Vector3<float> m_voxels;
    Vector3<glm::vec4> m_gradients;
    std::vector<unsigned char> m_data;
    std::array<float, 256> m_histogram;
    glm::vec3 m_voxelSize;
    glm::ivec3 m_res;
    float m_lastP=0;
    char m_lastAxis=0;
    VoxelTable m_voxelTable;
    std::vector<int> m_orders;
    bool m_isOpen;
    glm::vec3 m_ratio;
    friend class IsoSurface;
    friend class CrossSection;
};
