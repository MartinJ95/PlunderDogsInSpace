#pragma once
#include "../Dependencies/Include/glm/glm/glm.hpp"

class RigidBody
{
public:
	RigidBody();
	void ApplyPhysics(const float DeltaTime);
	glm::vec3 GetPosition() const;
	void SetForce(const glm::vec3& NewForce);
	void AddForce(const glm::vec3& AddedForce);
public:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_force;
	glm::vec3 m_constantForce;
};

