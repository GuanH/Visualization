#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
class VoxelModel;
class CrossSection{
public:
    CrossSection(const VoxelModel* model);
    void FreeBuffers();
    void UpdateCrossSection(const float p, const int axis);
    void UpdateOpaque(const float opaque);
    void DrawCrossSection();

private:
    const VoxelModel* r_model;
    std::vector<float> m_img;
    GLuint m_tex;
    glm::vec4 m_dim;
    float m_opaque;
};
