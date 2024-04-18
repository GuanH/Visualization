#include "App.h"
#include "glm/gtc/constants.hpp"
#include <glm/glm.hpp>

void App::Update(float deltaTime){
    float cameraSpeed = 5;
    float rotationFactor = 0.2;
    constexpr float twoPi = glm::two_pi<float>();
    constexpr float PiTwo = glm::half_pi<float>();
    if (m_window.KeyDown(GLFW_KEY_L)) {
        m_camera.m_yaw += deltaTime * cameraSpeed * rotationFactor;
        if (m_camera.m_yaw > twoPi) m_camera.m_yaw -= twoPi;
    }
    if (m_window.KeyDown(GLFW_KEY_H)) {
        m_camera.m_yaw -= deltaTime * cameraSpeed * rotationFactor;
        if (m_camera.m_yaw < 0) m_camera.m_yaw += twoPi;
    }
    if (m_window.KeyDown(GLFW_KEY_K)) {
        m_camera.m_pitch += deltaTime * cameraSpeed * rotationFactor;
        if (m_camera.m_pitch > PiTwo - 0.01) m_camera.m_pitch = PiTwo - 0.01;
    }
    if (m_window.KeyDown(GLFW_KEY_J)) {
        m_camera.m_pitch -= deltaTime * cameraSpeed * rotationFactor;
        if (m_camera.m_pitch < -PiTwo + 0.01) m_camera.m_pitch = -PiTwo + 0.01;
    }
    if (m_window.KeyDown(GLFW_KEY_I)) {
        m_camera.m_roll += deltaTime * cameraSpeed * rotationFactor;
        if (m_camera.m_roll > twoPi) m_camera.m_roll -= twoPi;
    }
    if (m_window.KeyDown(GLFW_KEY_N)) {
        m_camera.m_roll -= deltaTime * cameraSpeed * rotationFactor;
        if (m_camera.m_roll < 0)m_camera.m_roll += twoPi;
    }
    const glm::vec3 forward = m_camera.GetForwardDir();
    const glm::vec3 rightward = m_camera.GetRightwardDir();
    if (m_window.KeyDown(GLFW_KEY_W)) {
        m_camera.m_pos += forward * cameraSpeed * deltaTime;
    }
    if (m_window.KeyDown(GLFW_KEY_S)) {
        m_camera.m_pos -= forward * cameraSpeed * deltaTime;
    }
    if (m_window.KeyDown(GLFW_KEY_D)) {
        m_camera.m_pos += rightward * cameraSpeed * deltaTime;
    }
    if (m_window.KeyDown(GLFW_KEY_A)) {
        m_camera.m_pos -= rightward * cameraSpeed * deltaTime;
    }
    if (m_window.KeyDown(GLFW_KEY_E)) {
        m_camera.m_pos.y += cameraSpeed * deltaTime;
    }
    if (m_window.KeyDown(GLFW_KEY_Q)) {
        m_camera.m_pos.y -= cameraSpeed * deltaTime;
    }
}
