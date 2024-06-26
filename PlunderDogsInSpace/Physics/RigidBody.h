#pragma once
#include "../Dependencies/Include/glm/glm/glm.hpp"
#include "../Dependencies/Include/glm/glm/gtc/quaternion.hpp"

struct Transform
{
public:
	Transform();
	void EndFrame();
	const glm::mat4& GetModelXform() const;
	void CheckModelXForm();
	void SetPosition(const glm::vec3& NewPosition);
	void Move(const glm::vec3& MovementVector);
	const glm::vec3& GetPosition() const;
	void SetRotation(const glm::quat& NewRotation);
	const glm::quat& GetRotation() const;
	void SetScale(const glm::vec3& Scale)
	{
		scale = Scale;
		dirtyXForm = true;
	}
	glm::vec3 GetForwardVector() const
	{
		glm::quat q = glm::quat(0, 0, 0, 1);
		glm::quat rotator = rotation;
		q = q * rotation;
		q = q * glm::conjugate(rotator);
		//return glm::vec3(q.x, q.y, q.z);
		return glm::vec3(0, 0, 1) * rotation;
	}
private:
	void CalculateXForm();
private:
	glm::vec3 position;
	glm::vec3 newPosition;
	glm::vec3 scale;
	glm::quat rotation;
	glm::mat4 modelXForm;
	bool dirtyXForm;
};

class RigidBody
{
public:
	RigidBody();
	void ApplyPhysics(const float DeltaTime, Transform& T);
	void SetForce(const glm::vec3& NewForce);
	void AddForce(const glm::vec3& AddedForce);
	void SubtractForce(const glm::vec3& SubtractedForce);
	void SetMass(const float NewMass);
	float GetMass() const;
private:
	inline glm::vec3 GetVelocityChange(const float DeltaTime) const;
	inline glm::vec3 ApplyRotation(const glm::vec3& VelocityChange, Transform& T);
private:
	glm::vec3 m_velocity;
	glm::vec3 m_force;
	glm::vec3 m_constantForce;
	float m_mass;
};

