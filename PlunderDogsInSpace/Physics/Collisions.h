#pragma once
#include "../Dependencies/Include/glm/glm/glm.hpp"
#include "../Physics/RigidBody.h"
#include <iostream>
#include <stdlib.h>

class RigidBody;

struct CollisionData 
{
	CollisionData() : body(nullptr), t(nullptr), other(nullptr), otherT(nullptr), penetrationDepth(0.f), collisionNormal(), pointOfCollision(glm::vec3(0.f)), hasHit(false)
	{}
	RigidBody* body;
	Transform* t;
	RigidBody* other;
	Transform* otherT;
	float penetrationDepth;
	glm::vec3 collisionNormal;
	glm::vec3 pointOfCollision;
	bool hasHit;
	friend std::ostream& operator<<(std::ostream& os, const CollisionData& data);
};

inline std::ostream& operator<<(std::ostream& os, const CollisionData& data)
{
	system("CLS");
	os << "Penetration Depth: " << data.penetrationDepth << "\n"
		<< "CollisionNormal:" << "\nx: " << data.collisionNormal.x << "\ny: " << data.collisionNormal.y << "\nz: " << data.collisionNormal.z << "\n"
		<< "PointOfCollision:" << "\nx: " << data.pointOfCollision.x << "\ny: " << data.pointOfCollision.y << "\nz: " << data.pointOfCollision.z << "\n"
		<< "Hit: " << data.hasHit << "\n";
	return os;
}

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
	if (Data.otherT != nullptr)
	{
		PlanePos = Data.otherT->GetPosition();
	}

	float originDot = glm::dot(Plane.planeNormal, Raycast.origin - PlanePos);
	float destDot = glm::dot(Plane.planeNormal, (Raycast.origin + Raycast.line) - PlanePos);

	if ((originDot < 0.f && destDot > 0.f) || (originDot > 0.f && destDot < 0.f))
	{
		Data.hasHit = true;
		Data.collisionNormal = Plane.planeNormal;

		glm::vec3 DestToPlaneOrigin = PlanePos - (Raycast.origin + Raycast.line);
		glm::vec3 DestToOrigin = Raycast.origin - (Raycast.origin + Raycast.line);

		Data.penetrationDepth = glm::dot(Plane.planeNormal, DestToPlaneOrigin);

		Data.pointOfCollision = ((Raycast.origin + (glm::normalize(Raycast.line) * (glm::length(Raycast.line) - Data.penetrationDepth))));
	}
}
static void RayToSphere(const Ray& Raycast, const SphereCollider& Sphere, CollisionData& Data);