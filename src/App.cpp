#include "App.h"
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include "glad/glad.h"
#include "glm/ext/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

App::App():m_window(1600, 1200, "Iso Surface"){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window.GetWnd(), true);
    ImGui_ImplOpenGL3_Init();

    m_defaultShader
        .SetVertexShader("shaders/default.vert")
        //.SetGeometryShader("shaders/default.geom")
        .SetFragmentShader("shaders/default.frag")
        .Done();

    m_planeShader
        .SetVertexShader("shaders/plane.vert")
        .SetFragmentShader("shaders/plane.frag")
        .Done();

    m_volumeShader
        .SetVertexShader("shaders/volume.vert")
        .SetFragmentShader("shaders/volume.frag")
        .Done();

    m_defaultShader.Use();
    ReloadAssets();
    m_tmod = new VoxelModel("", "");
    m_transfer.fill(0);
}

void App::Run(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_timer.newFrame();
    m_windowFlag = ImGuiWindowFlags_None;
    while(m_window.ProcessEvent()){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_timer.newFrame();
        Update(m_timer.getDuration());
        DrawWorld();
        DrawImGui();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        m_window.SwapBuffers();
        usleep(16);
    }
}


#include <imgui_internal.h>
void App::DrawImGuiTransfer(){
    const char* label = "TEST LABEL";
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow();
    int id = window->GetID(label);
    ImGuiContext& g = *GImGui;
    const ImVec2 pos = window->DC.CursorPos;
    const float eleWidth = 2;
    const float gap = 1;
    const int eleNum = 256;
    const float height = 200;
    const ImVec2 sz = ImVec2((eleWidth + gap) * eleNum, height);
    const ImRect bound(pos, pos + sz);
    ItemSize(bound, 0);
    ItemAdd(bound, id);
    bool hovered, held;
    bool pressed = ButtonBehavior(bound, id, &hovered, &held);
    RenderFrame(bound.Min, bound.Max, GetColorU32(ImGuiCol_FrameBg));
    int hoverId = -1 ;
    for(int i=0;i<eleNum;i++){
        float v = m_transfer[i] * height;
        auto p0 = ImVec2(bound.Min.x + i*(eleWidth+gap), bound.Max.y - v);
        auto p1 = ImVec2(bound.Min.x + i*(eleWidth+gap)+eleWidth, bound.Max.y);
        auto p2 = ImVec2(bound.Min.x + i*(eleWidth+gap), bound.Min.y);
        auto p3 = ImVec2(bound.Min.x + (i+1)*(eleWidth+gap), bound.Max.y);
        if(ImRect(p2,p3).Contains(g.IO.MousePos)){
            hoverId = i;
        }
        window->DrawList->AddRectFilled(p0, p1, GetColorU32(ImGuiCol_PlotHistogram));
    }
    static float radius = 5;
    if(hovered){
        if(m_window.GetScrollY()){
            radius = fmax(radius + m_window.GetScrollY(), 0.0f);
        }
        window->DrawList->AddCircleFilled(g.IO.MousePos, radius * 5.0f, GetColorU32(ImGuiCol_FrameBgHovered), 16);
        m_windowFlag |= ImGuiWindowFlags_NoScrollWithMouse;
    }
    static float firstX;
    static float lastY;
    if(pressed) firstX = g.IO.MousePos.x;
    if(held && hoverId != -1){
        float dy = lastY - g.IO.MousePos.y;
        const float rate = 0.01f;
        for(int i=0;i<256;i++){
            float x = (i - hoverId) / radius;
            float str = exp(-x*x) * rate;
            m_transfer[i] = std::clamp(m_transfer[i] + dy * str, 0.0f, 1.0f);
        }
        m_tmod->UpdateTransfer(m_transfer);
    }
    lastY = g.IO.MousePos.y;
}

void App::DrawImGui(){
    //ImGui::ShowDemoWindow();
    ImGui::Begin("Config",0,m_windowFlag);
    m_windowFlag = ImGuiWindowFlags_None;

    if(ImGui::Button("↻")){
        ReloadAssets();
    };
    if(ImGui::BeginCombo("Info File", m_selectedInfo==-1?"":m_infoFiles[m_selectedInfo].c_str())){
        for(int i=0;i<m_infoFiles.size();i++){
            bool sel = i == m_selectedInfo;
            if(ImGui::Selectable(m_infoFiles[i].c_str(), sel)){
                m_selectedInfo = i;
            }
            if(sel){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if(ImGui::BeginCombo("Raw File", m_selectedRaw==-1?"":m_rawFiles[m_selectedRaw].c_str())){
        for(int i=0;i<m_rawFiles.size();i++){
            bool sel = i == m_selectedRaw;
            if(ImGui::Selectable(m_rawFiles[i].c_str(), sel)){
                m_selectedRaw = i;
            }
            if(sel){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if(ImGui::Button("Open")){
        if(m_selectedInfo!=-1&&m_selectedRaw!=-1){
            delete m_tmod;
            m_tmod = new VoxelModel(m_infoFiles[m_selectedInfo].c_str(), m_rawFiles[m_selectedRaw].c_str());
            m_transfer.fill(0);

            m_surfaceInfoes.clear();
            // m_surfaceInfoes.push_back({128.0f, 1.0f});
            // m_tmod->AddSurface();
            // m_tmod->UpdateMarchingTetra(0, 0.5f);
            // m_tmod->UpdateMarchingTetraOpaque(0, 1.0f);

            m_crossSectionInfoes.clear();
            // m_crossSectionInfoes.push_back({0,0,1.0f});
            // m_tmod->AddCrossSection();
            // m_tmod->UpdateCrossSection(0, 0, 0);
            // m_tmod->UpdateCrossSectionOpaque(0, 1.0f);
        }
    }
    ImGui::NewLine();
    DrawImGuiTransfer();
    if(ImGui::Button("Smooth")){
        std::array<float,256> tem;
        tem.fill(0);
        const int radius = 5;
        for(int i=0;i<256;i++){
            int cnt = 0;
            for(int j=-radius;j<radius;j++){
                int x = i+j;
                if(x<0||x>=256)continue;
                tem[i]+=m_transfer[x];
                cnt++;
            }
            if(cnt){
                tem[i] = tem[i]/cnt;
            }
        }
        m_transfer = tem;
    }
    ImGui::NewLine();
    if(ImGui::Button("Add Surface")){
        if(m_tmod->IsOpen()){
            m_surfaceInfoes.push_back({255.0f, 1.0f});
            m_tmod->AddSurface();
            m_tmod->UpdateMarchingTetra(m_surfaceInfoes.size()-1, 1.0f);
            m_tmod->UpdateMarchingTetraOpaque(m_surfaceInfoes.size()-1, 1.0f);
        }
    }
    ImGui::NewLine();
    char lab[]="###a0";
    for(int i=0;i<m_surfaceInfoes.size();i++){
        lab[4]='0'+i;
        lab[0]='x';
        lab[3]='a';
        if(ImGui::Button(lab)){
            m_surfaceInfoes.erase(m_surfaceInfoes.begin()+i);
            m_tmod->DeleteSurface(i);
            break;
        }
        lab[0]='#';
        ImGui::Text("Iso Value : ");
        ImGui::SameLine();
        if(ImGui::SliderFloat(lab, &m_surfaceInfoes[i].value, 0.0f, 255.0f)){
            m_tmod->UpdateMarchingTetra(i, m_surfaceInfoes[i].value / 255.0f);
        }
        lab[3]='b';
        ImGui::Text("Opaque    : ");
        ImGui::SameLine();
        if(ImGui::SliderFloat(lab, &m_surfaceInfoes[i].opaque, 0.0f, 1.0f)){
            m_tmod->UpdateMarchingTetraOpaque(i, m_surfaceInfoes[i].opaque);
        }
        ImGui::NewLine();
    }
    ImGui::NewLine();
    if(ImGui::Button("Add Cross-Section")){
        if(m_tmod->IsOpen()){
            m_crossSectionInfoes.push_back({0, 0, 1.0f});
            m_tmod->AddCrossSection();
            m_tmod->UpdateCrossSection(m_crossSectionInfoes.size()-1, 0, 0);
            m_tmod->UpdateCrossSectionOpaque(m_crossSectionInfoes.size()-1, 1.0f);
        }
    }
    for(int i=0;i<m_crossSectionInfoes.size();i++){
        lab[4]='0'+i;
        lab[0]='x';
        lab[3]='c';
        if(ImGui::Button(lab)){
            m_crossSectionInfoes.erase(m_crossSectionInfoes.begin()+i);
            m_tmod->DeleteCrossSection(i);
            break;
        }
        lab[0]='#';
        const char* xyz[]={"x", "y", "z"};
        ImGui::Text("Axis      : ");
        ImGui::SameLine();
        if(ImGui::BeginCombo(lab, xyz[m_crossSectionInfoes[i].axis])){
            for(int j=0;j<3;j++){
                bool sel = j == m_crossSectionInfoes[i].axis;
                if(ImGui::Selectable(xyz[j], sel)){
                    m_crossSectionInfoes[i].axis = j;
                    m_tmod->UpdateCrossSection(i, m_crossSectionInfoes[i].value, m_crossSectionInfoes[i].axis);
                }
                if(sel){
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        lab[3]='d';
        ImGui::Text("Value     : ");
        ImGui::SameLine();
        if(ImGui::SliderFloat(lab, &m_crossSectionInfoes[i].value, 0.0f, 1.0f)){
            m_tmod->UpdateCrossSection(i, m_crossSectionInfoes[i].value, m_crossSectionInfoes[i].axis);
        }
        lab[3]='e';
        ImGui::Text("Opaque :    ");
        ImGui::SameLine();
        if(ImGui::SliderFloat(lab, &m_crossSectionInfoes[i].opaque, 0.0f, 1.0f)){
            m_tmod->UpdateCrossSectionOpaque(i, m_crossSectionInfoes[i].opaque);
        }
        ImGui::NewLine();
    }
    ImGui::NewLine();
    ImGui::Text("Histogram : ");
    ImGui::PlotHistogram("##Histogram", m_tmod->GetHistogram().data(), m_tmod->GetHistogram().size(),0,NULL,0.0f,FLT_MAX,ImVec2(400,300));
    ImGui::End();
}

void App::DrawWorld(){
    m_volumeShader.Use();
    auto invViewProj = glm::inverse(m_camera.GetProjectionMatrix(m_window.GetWndWidth(), m_window.GetWndHeight())*
    m_camera.GetViewMatrix());
    glUniformMatrix4fv(0,1,GL_FALSE,glm::value_ptr(invViewProj));
    glUniform3fv(3,1,glm::value_ptr(m_camera.GetOrthoForward()));
    m_tmod->DrawVolume();
    return;

    m_defaultShader.Use();
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(glm::identity<glm::mat4>()));
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_camera.GetProjectionMatrix(m_window.GetWndWidth(), m_window.GetWndHeight()) * m_camera.GetViewMatrix()));
    m_tmod->DrawMarchingTetra();


    m_planeShader.Use();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_camera.GetProjectionMatrix(m_window.GetWndWidth(), m_window.GetWndHeight()) * m_camera.GetViewMatrix()));
    glUniform1i(2,0);
    m_tmod->DrawCrossSection();



    m_defaultShader.Use();
    glEnable(GL_CULL_FACE);
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(glm::identity<glm::mat4>()));
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_camera.GetProjectionMatrix(m_window.GetWndWidth(), m_window.GetWndHeight()) * m_camera.GetViewMatrix()));
    m_tmod->DrawMarchingTetra();

}

inline bool EndWith(const std::string& a, const std::string&b){
    auto it1=a.rbegin();
    auto it2=b.rbegin();
    for(;it1!=a.rend()&&it2!=b.rend();it1++,it2++){
        if(*it1!=*it2)return false;
    }
    return it2==b.rend();
}

void App::ReloadAssets(){
    m_infoFiles.clear();
    m_rawFiles.clear();
    for(const auto& file:std::filesystem::directory_iterator("./assets")){
        auto path = file.path().u8string();
        std::string str{path.begin(), path.end()};
        if(EndWith(str, ".inf")){
            m_infoFiles.push_back( std::string(str.begin()+9,str.end()));
        }
        else if(EndWith(str, ".raw")){
            m_rawFiles.push_back( std::string(str.begin()+9,str.end()));
        }
    }
    std::sort(m_infoFiles.begin(), m_infoFiles.end());
    std::sort(m_rawFiles.begin(), m_rawFiles.end());
}
