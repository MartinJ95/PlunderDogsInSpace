#pragma once
#include "../Dependencies/Include/glm/glm/glm.hpp"
#include "../Dependencies/Include/glm/glm/gtc/quaternion.hpp"

struct Transform
{
	void EndFrame();
	glm::vec3 position;
	glm::vec3 newPosition;
	glm::quat rotation;
};

class RigidBody
{
public:
	RigidBody();
	void ApplyPhysics(const float DeltaTime);
	const Transform& GetTransform() const;
	void SetForce(const glm::vec3& NewForce);
	void AddForce(const glm::vec3& AddedForce);
	void SubtractForce(const glm::vec3& SubtractedForce);
private:
	inline glm::vec3 GetVelocityChange(const float DeltaTime) const;
	inline glm::vec3 ApplyRotation(const glm::vec3& VelocityChange);
private:
	Transform m_transform;
	glm::vec3 m_velocity;
	glm::vec3 m_force;
	glm::vec3 m_constantForce;
	float m_mass;
};

