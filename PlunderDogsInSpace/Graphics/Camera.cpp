#include "Camera.h"

Camera::Camera() : m_position(glm::vec3(0.f, 0.f, 0.f)), m_rotation(), m_fov(45.f)
{
}

glm::vec3 Camera::GetViewDir() const
{
	glm::quat q(0.f, 0.f, 0.f, -1.f);
	q = m_rotation * q;
	q = q * glm::conjugate(m_rotation);
	return glm::vec3(q.x, q.y, q.z);
}

glm::vec3 Camera::GetViewDirOnScreen(const glm::vec2& ScreenSize, const glm::vec2& MousePosition) const
{
	glm::quat q(0.f, 0.f, 0.f, -1.f);
	glm::quat rotator = m_rotation;

	float xRotation = m_fov * (MousePosition.y / ScreenSize.y);
	xRotation -= m_fov * 0.5;
	xRotation *= -1;

	float yRotation = m_fov * (MousePosition.x / ScreenSize.x);
	yRotation -= m_fov * 0.5;
	yRotation *= -1;
	yRotation *= 1 + (ScreenSize.y / ScreenSize.x);

	rotator *= glm::quat(glm::vec3(glm::radians(xRotation), 0.f, 0.f));
	rotator *= glm::quat(glm::vec3(0.f, glm::radians(yRotation), 0.f));

	q = rotator * q;
	q = q * glm::conjugate(rotator);
	return glm::vec3(q.x, q.y, q.z);
}

glm::vec3 Camera::GetCameraUp() const
{
	glm::vec3 camDir = glm::normalize(GetViewDir());
	float d = glm::dot(camDir, glm::vec3(1, 0, 0));
	glm::vec3 side = d < -0.8f || d > 0.8f ? glm::vec3(0, 0, 1) : glm::vec3(1, 0, 0);
	return glm::normalize(glm::cross(camDir, side));
}

float Camera::GetFOV() const
{
	return m_fov;
}