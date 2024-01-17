#pragma once
#include "../Dependencies/Include/glm/glm/glm.hpp"
#include "../Dependencies/Include/glm/glm/gtc/quaternion.hpp"

class RigidBody
{
public:
	RigidBody();
	void ApplyPhysics(const float DeltaTime, const glm::quat& Rotation);
	const glm::vec3& GetPosition() const;
	void SetForce(const glm::vec3& NewForce);
	void AddForce(const glm::vec3& AddedForce);
	void SubtractForce(const glm::vec3& SubtractedForce);
private:
	inline glm::vec3 GetVelocityChange(const float DeltaTime) const;
	inline glm::vec3 ApplyRotation(const glm::vec3& VelocityChange, const glm::quat& Rotation);
private:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_force;
	glm::vec3 m_constantForce;
	float m_mass;
};

