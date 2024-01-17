#pragma once
#include "../Dependencies/Include/glm/glm/glm.hpp"

class RigidBody;

struct CollisionData 
{
	RigidBody* body;
	RigidBody* other;
	float penetrationDepth;
	glm::vec3 collisionNormal;
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