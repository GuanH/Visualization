#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
class Camera {
public:
	Camera();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(int wndWidth, int wndHeight);
	glm::vec3 GetForwardDir();
	glm::vec3 GetRightwardDir();
	glm::vec3 GetOrthoForward();
	glm::vec3 m_pos;
	float m_roll, m_pitch, m_yaw;
	float m_zoom;
	float m_fov;
	float m_radius;
private:
};
