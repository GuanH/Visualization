#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
Camera::Camera()
	:m_roll(0), m_pitch(0), m_yaw(0), m_zoom(1), m_pos({0,0,0}),m_fov(90) {}

glm::mat4 Camera::GetViewMatrix() {
	glm::vec3 focusDir = GetForwardDir();
	glm::vec3 focus = focusDir + m_pos;
	glm::vec3 rightDir = glm::normalize(glm::cross(focusDir, glm::vec3{ 0,1,0 }));
	glm::vec3 up = glm::vec3{ 0,std::cos(m_roll),0 } + std::sin(m_roll) * rightDir;

	return glm::lookAt(m_pos, focus, up);
}

glm::mat4 Camera::GetProjectionMatrix(int wndWidth, int wndHeight) {
	float fov = glm::clamp(m_fov * m_zoom, 1.f, 179.f);
	return glm::perspectiveFov(fov, (float)wndWidth, (float)wndHeight, 0.001f, 50.f);
}

glm::vec3 Camera::GetForwardDir() {
	return { std::cos(m_yaw) * std::cos(m_pitch),
		std::sin(m_pitch),
		std::sin(m_yaw) * std::cos(m_pitch)
	};
}

glm::vec3 Camera::GetRightwardDir() {
	glm::vec3 focusDir = GetForwardDir();
	return glm::normalize(glm::cross(focusDir, glm::vec3{ 0,1,0 }));
}
