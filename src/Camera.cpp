#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
Camera::Camera()
	:m_roll(0), m_pitch(0), m_yaw(0), m_zoom(1), m_pos({0,0,0}),m_fov(90),m_radius(2) {}

glm::mat4 Camera::GetViewMatrix() {
	m_pos = GetForwardDir() * 0.87f;
	return glm::lookAt(m_pos, glm::vec3(0), glm::vec3(0,1,0));
}

glm::mat4 Camera::GetProjectionMatrix(int wndWidth, int wndHeight) {
	float right = m_radius;
	float top = right * wndHeight / wndWidth;
	right *= 0.5f;
	top *= 0.5f;
	return glm::ortho(-right, right, -top, top, 0.0f, 1.8f);
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

glm::vec3 Camera::GetOrthoForward(){
	return -glm::normalize(m_pos);
}
