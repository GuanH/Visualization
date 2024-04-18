#pragma once
#include "Window.h"
#include "Camera.h"
#include "Timer.h"
#include "VoxelModel.h"
#include "ShaderProgram.h"
#include <vector>
#include <string>

struct SurfaceInfo{
    float value;
    float opaque;
};

struct CrossSectionInfo{
    char axis;
    float value;
    float opaque;
};

class App{
public:
    App();
    void Run();
private:
    void DrawImGui();
    void DrawWorld();
    void Update(float deltaTime);
    void ReloadAssets();
    Timer m_timer;
    Window m_window;
    Camera m_camera;
    VoxelModel* m_tmod;
    ShaderProgram m_defaultShader;
    ShaderProgram m_planeShader;
    float m_p = 0.5f;
    std::vector<std::string> m_infoFiles;
    std::vector<std::string> m_rawFiles;
    int m_selectedInfo=-1;
    int m_selectedRaw=-1;
    std::vector<SurfaceInfo> m_surfaceInfoes;
    std::vector<CrossSectionInfo> m_crossSectionInfoes;
};
