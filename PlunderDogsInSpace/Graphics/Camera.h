#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/quaternion.hpp" 
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
public:
	glm::vec3 GetPos() const { return m_position; }
	void SetPos(const glm::vec3& NewPos) { m_position = NewPos; }
	glm::quat GetRotation() const { return m_rotation; }
	void SetRotation(const glm::quat& NewRot) { m_rotation = NewRot; }
	glm::vec3 GetViewDir() const;
	glm::vec3 GetViewDirOnScreen(const glm::vec2& ScreenSize, const glm::vec2& MousePosition) const;
	glm::vec3 GetCameraUp() const;
	float GetFOV() const;
protected:
	glm::vec3 m_position;
	glm::quat m_rotation;
	float m_fov;
};