#include "Collisions.h"
#include "../Physics/RigidBody.h"

void RayToPlane(const Ray& Raycast, const PlaneCollider& Plane, CollisionData& Data)
{
	glm::vec3 PlanePos = glm::vec3(0.f);
	if (Data.other != nullptr)
	{
		PlanePos = Data.other->GetTransform().position;
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

		Data.penetrationDepth =  glm::length(PlanePos - (Raycast.origin + Raycast.line)) * percentage;

		Data.pointOfCollision = (Raycast.origin + (Raycast.line * (glm::length(Raycast.line) - Data.penetrationDepth)));
	}
}
