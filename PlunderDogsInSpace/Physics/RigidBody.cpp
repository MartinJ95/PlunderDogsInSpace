#include "RigidBody.h"

RigidBody::RigidBody() : m_velocity(glm::vec3(0.f)), m_force(glm::vec3(0.f)), m_constantForce(glm::vec3(0.f, 0.f, 0.f)), m_mass(10.f)
{
}

void RigidBody::ApplyPhysics(const float DeltaTime, Transform& T)
{
	glm::vec3 VelocityChange = GetVelocityChange(DeltaTime);
	ApplyRotation(VelocityChange, T);

	m_velocity += VelocityChange;

	m_velocity *= 0.9f;

	T.Move(m_velocity);
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

inline glm::vec3 RigidBody::ApplyRotation(const glm::vec3& VelocityChange, Transform& T)
{
	return T.GetRotation() * VelocityChange * glm::conjugate(T.GetRotation());
}

Transform::Transform() : position(glm::vec3(0.f)), newPosition(glm::vec3(0.f)), scale(glm::vec3(1.f)), rotation(glm::quat(glm::vec3(0.f, 0.f, 0.f))), modelXForm() , dirtyXForm(true)
{
}

void Transform::EndFrame()
{
	if (position != newPosition)
	{
		dirtyXForm = true;
	}
	position = newPosition;
}

const glm::mat4& Transform::GetModelXform() const
{
	/*
	if (dirtyXForm)
	{
		CalculateXForm();
	}
	*/
	return modelXForm;
}

void Transform::CheckModelXForm()
{
	if (dirtyXForm)
	{
		CalculateXForm();
	}
}

void Transform::SetPosition(const glm::vec3& NewPosition)
{
	newPosition = NewPosition;
}

void Transform::Move(const glm::vec3& MovementVector)
{
	newPosition += MovementVector;
}

const glm::vec3& Transform::GetPosition() const
{
	return position;
}

void Transform::SetRotation(const glm::quat& NewRotation)
{
	if (rotation == NewRotation)
	{
		return;
	}
	rotation = NewRotation;
	dirtyXForm = true;
}

const glm::quat& Transform::GetRotation() const
{
	return rotation;
}

void Transform::CalculateXForm()
{
	glm::mat4 newMat = glm::mat4(1.f);
	newMat = glm::translate(newMat, position);
	//newMat * glm::mat4_cast(rotation);
	newMat = glm::scale(newMat, scale);
	modelXForm = newMat;
	dirtyXForm = false;
}
