#include "RigidBody.h"

RigidBody::RigidBody() : m_position(glm::vec3(0.f)), m_velocity(glm::vec3(0.f)), m_force(glm::vec3(0.f)), m_constantForce(glm::vec3(0.f, 0.f, 0.f))
{
}
