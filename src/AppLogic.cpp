#include "App.h"
#include "glm/gtc/constants.hpp"
#include <glm/glm.hpp>

void App::Update(float deltaTime){
    float cameraSpeed = 5;
    float rotationFactor = 0.2;
    constexpr float twoPi = glm::two_pi<float>();
    constexpr float PiTwo = glm::half_pi<float>();
    if (m_window.KeyDown(GLFW_KEY_A)) {
        m_camera.m_yaw += deltaTime * cameraSpeed * rotationFactor;
        if (m_camera.m_yaw > twoPi) m_camera.m_yaw -= twoPi;
    }
    if (m_window.KeyDown(GLFW_KEY_D)) {
        m_camera.m_yaw -= deltaTime * cameraSpeed * rotationFactor;
        if (m_camera.m_yaw < 0) m_camera.m_yaw += twoPi;
    }
    if (m_window.KeyDown(GLFW_KEY_E)) {
        m_camera.m_pitch += deltaTime * cameraSpeed * rotationFactor;
        if (m_camera.m_pitch > PiTwo - 0.01) m_camera.m_pitch = PiTwo - 0.01;
    }
    if (m_window.KeyDown(GLFW_KEY_Q)) {
        m_camera.m_pitch -= deltaTime * cameraSpeed * rotationFactor;
        if (m_camera.m_pitch < -PiTwo + 0.01) m_camera.m_pitch = -PiTwo + 0.01;
    }
    if (m_window.KeyDown(GLFW_KEY_W)) {
        m_camera.m_radius -= cameraSpeed * deltaTime;
        m_camera.m_radius = std::max(m_camera.m_radius, 0.0f);
    }
    if (m_window.KeyDown(GLFW_KEY_S)) {
        m_camera.m_radius += cameraSpeed * deltaTime;
    }
}
