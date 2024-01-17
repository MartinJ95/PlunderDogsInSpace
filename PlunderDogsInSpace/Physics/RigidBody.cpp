#include "RigidBody.h"

RigidBody::RigidBody() : m_position(glm::vec3(0.f)), m_velocity(glm::vec3(0.f)), m_force(glm::vec3(0.f)), m_constantForce(glm::vec3(0.f, 0.f, 0.f)), m_mass(10.f)
{
}

void RigidBody::ApplyPhysics(const float DeltaTime, const glm::quat& Rotation)
{
	glm::vec3 VelocityChange = GetVelocityChange(DeltaTime);
	ApplyRotation(VelocityChange, Rotation);

	m_velocity += VelocityChange;

	m_velocity *= 0.9f;

	m_position += m_velocity;
}

const glm::vec3& RigidBody::GetPosition() const
{
	return m_position;
}

void RigidBody::SetForce(const glm::vec3& NewForce)
{
	m_force = NewForce;
}

void RigidBody::AddForce(const glm::vec3& AddedForce)
{
	m_force += AddedForce;
}

void RigidBody::SubtractForce(const glm::vec3& SubtractedForce)
{
	m_force -= SubtractedForce;
}

inline glm::vec3 RigidBody::GetVelocityChange(const float DeltaTime) const
{
	glm::vec3 acceleration = (m_force + m_constantForce) / m_mass;
	return acceleration * DeltaTime;
}

inline glm::vec3 RigidBody::ApplyRotation(const glm::vec3& VelocityChange, const glm::quat& Rotation)
{
	return Rotation * VelocityChange * glm::conjugate(Rotation);
}
