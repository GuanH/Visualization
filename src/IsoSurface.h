#pragma once
#include "Vector3.h"
#include "Vec3Table.h"
#include <glad/glad.h>
#include "Utility.h"

class VoxelModel;
class IsoSurface{
public:
    IsoSurface(const VoxelModel* model);
    void FreeBuffers();
    void UpdateMarchingTetra(const float t);
    void UpdateMarchingTetraOpaque(const float t);
    void DrawMarchingTetra();
    float GetIsoValue()const{return m_t;}

private:
    void DrawTetra(const glm::vec4&a, const glm::vec4&b, const glm::vec4&c, const glm::vec4&d);
    void DrawSplitCubeA(const glm::vec4&a,const glm::vec4&b, const glm::vec4&c, const glm::vec4&d, const glm::vec4&e, const glm::vec4&f, const glm::vec4&g, const glm::vec4&h);
    void DrawSplitCubeB(const glm::vec4&a,const glm::vec4&b, const glm::vec4&c, const glm::vec4&d, const glm::vec4&e, const glm::vec4&f, const glm::vec4&g, const glm::vec4&h);
    const VoxelModel* r_model;
    Vec3Table m_vertexTable;
    std::vector<Vertex> m_vertices;
    std::vector<int> m_indices;
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    float m_t;
    glm::vec4 m_color;
};
