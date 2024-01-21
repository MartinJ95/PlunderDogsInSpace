#pragma once
#include "../Dependencies/Include/glm/glm/glm.hpp"
#include "../Physics/RigidBody.h"

class RigidBody;

struct CollisionData 
{
	RigidBody* body;
	RigidBody* other;
	float penetrationDepth;
	glm::vec3 collisionNormal;
	glm::vec3 pointOfCollision;
	bool hasHit;
};

struct Ray
{
	glm::vec3 origin;
	glm::vec3 line;
};

struct AABB
{
	glm::vec2 min;
	glm::vec2 max;
};

struct SphereCollider
{
	float radius;
};

struct PlaneCollider
{
	glm::vec3 planeNormal;
};


//raycast functions
static void RayToPlane(const Ray& Raycast, const PlaneCollider& Plane, CollisionData& Data)
{
	glm::vec3 PlanePos = glm::vec3(0.f);
	if (Data.other != nullptr)
	{
		PlanePos = Data.other->GetTransform().GetPosition();
	}

	float originDot = glm::dot(Plane.planeNormal, Raycast.origin - PlanePos);
	float destDot = glm::dot(Plane.planeNormal, (Raycast.origin + Raycast.line) - PlanePos);

	if ((originDot < 0.f && destDot > 0.f) || (originDot > 0.f && destDot < 0.f))
	{
		Data.hasHit = true;
		Data.collisionNormal = Plane.planeNormal;

		glm::vec3 DestToPlaneOrigin = glm::normalize(PlanePos - (Raycast.origin + Raycast.line));
		glm::vec3 DestToOrigin = glm::normalize(Raycast.origin - (Raycast.origin + Raycast.line));

		float percentage = glm::dot(DestToPlaneOrigin, DestToOrigin);

		Data.penetrationDepth = glm::length(PlanePos - (Raycast.origin + Raycast.line)) * percentage;

		Data.pointOfCollision = (Raycast.origin + (glm::normalize(Raycast.line) * (glm::length(Raycast.line) - Data.penetrationDepth)));
	}
}
static void RayToSphere(const Ray& Raycast, const SphereCollider& Sphere, CollisionData& Data);