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
	SphereCollider() : radius(0.f)
	{}
	SphereCollider(float Radius) : radius(Radius)
	{}
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

		glm::vec3 DestToPlaneFace = Plane.planeNormal * glm::dot(Plane.planeNormal, DestToPlaneOrigin);

		glm::vec3 DestToCollisionPoint = glm::normalize(DestToOrigin) * glm::dot(glm::normalize(DestToOrigin), DestToPlaneFace);

		Data.penetrationDepth = glm::length(DestToCollisionPoint);

		Data.pointOfCollision = (Raycast.origin + Raycast.line) + DestToCollisionPoint;

		//Data.penetrationDepth = glm::dot(Plane.planeNormal, DestToPlaneOrigin);

		//Data.pointOfCollision = ((Raycast.origin + (glm::normalize(Raycast.line) * (glm::length(Raycast.line) - Data.penetrationDepth))));
	}
}
static void RayToSphere(const Ray& Raycast, const SphereCollider& Sphere, CollisionData& Data);
static void SphereToSphere(const SphereCollider& obj, const SphereCollider& other, CollisionData& Data)
{
	glm::vec3 ObjToOther = Data.otherT->GetPosition() - Data.t->GetPosition();
	if (glm::length(ObjToOther) < obj.radius + other.radius)
	{
		Data.hasHit = true;
		Data.collisionNormal = glm::normalize(ObjToOther);
		Data.penetrationDepth = (obj.radius + other.radius) - glm::length(ObjToOther);
		Data.pointOfCollision = Data.otherT->GetPosition() + glm::normalize(-ObjToOther) * (Data.penetrationDepth + other.radius);
	}
}

static bool AABBContainsPoint(const glm::vec2& point, const AABB& box)
{
	if (point.x > box.min.x && point.x < box.max.x && point.y > box.min.y && point.y < box.max.y)
	{
		return true;
	}
	return false;
}