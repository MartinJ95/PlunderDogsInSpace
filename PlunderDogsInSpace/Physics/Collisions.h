#pragma once
#include "../Dependencies/Include/glm/glm/glm.hpp"

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
static void RayToPlane(const Ray& Raycast, const PlaneCollider& Plane, CollisionData& Data);
static void RayToSphere(const Ray& Raycast, const SphereCollider& Sphere, CollisionData& Data);