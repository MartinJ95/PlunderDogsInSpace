#include "CustomBTNodes.h"
#include "Team.h"
#include "Sandbox.h"

BTNodeResult BTShipFindTarget::Evaluate(void* ptr) const
{
	if (ptr == nullptr)
		return BTNodeResult::eBTFail;

	ShipAIData* data = static_cast<ShipAIData*>(ptr);

	if (data->targetShip == nullptr && data->owningTeam->GetOtherTeam()->GetShips().size() != 0)
	{
		float closest = 999999.f;

		Ship* closestShip = nullptr;

		for (Ship& s : data->owningTeam->GetOtherTeam()->GetShips())
		{
			float distance = glm::length(s.m_transform.GetPosition() - data->owner->m_transform.GetPosition());
			if (distance < closest)
			{
				closestShip = &s;
				closest = distance;
			}
		}
		data->targetShip = closestShip;
	}
	return BTNodeResult::eBTSuccess;
}

BTNodeResult BTShipSetMoveToLocation::Evaluate(void* ptr) const
{
	if (ptr == nullptr)
		return BTNodeResult::eBTFail;

	ShipAIData* data = static_cast<ShipAIData*>(ptr);

	Sandbox* s = (Sandbox*)ServiceLocator::GetMainService();

	if (data->targetShip == nullptr)
	{
		//data->targetLocation = data->owner->m_transform.GetPosition();
		data->targetLocation = s->GetTestingGrid().GetPointOnTile(data->owner->m_transform.GetPosition());
		return BTNodeResult::eBTSuccess;
	}

	if (data->owner->GetWeapon().weapon == nullptr)
	{
		//data->targetLocation = data->owner->m_transform.GetPosition();
		data->targetLocation = s->GetTestingGrid().GetPointOnTile(data->owner->m_transform.GetPosition());
		return BTNodeResult::eBTSuccess;
	}

	if (glm::length(data->targetShip->m_transform.GetPosition() - data->owner->m_transform.GetPosition()) < data->owner->GetWeapon().weapon->range)
	{
		//data->targetLocation = data->owner->m_transform.GetPosition();
		data->targetLocation = s->GetTestingGrid().GetPointOnTile(data->owner->m_transform.GetPosition());
		return BTNodeResult::eBTSuccess;
	}

	//data->targetLocation = data->targetShip->m_transform.GetPosition();
	data->targetLocation = s->GetTestingGrid().GetPointOnTile(data->targetShip->m_transform.GetPosition());
	return BTNodeResult::eBTSuccess;
}

BTNodeResult BTShipMoveToLocation::Evaluate(void* ptr) const
{
	if (ptr == nullptr)
		return BTNodeResult::eBTFail;

	ShipAIData* data = static_cast<ShipAIData*>(ptr);

	if (glm::length(data->targetLocation - data->owner->m_transform.GetPosition()) > 2.f)
	{
		data->owner->m_transform.Move(glm::normalize(data->targetLocation - data->owner->m_transform.GetPosition()) * ServiceLocator::GetTimeService().deltaTime);
	}

	return BTNodeResult::eBTSuccess;
}

BTNodeResult BTShipShootAtTarget::Evaluate(void* ptr) const
{
	if (ptr == nullptr)
		return BTNodeResult::eBTFail;

	ShipAIData* data = static_cast<ShipAIData*>(ptr);

	if (data->targetShip == nullptr)
		return BTNodeResult::eBTFail;

	if (glm::length(data->targetShip->m_transform.GetPosition() - data->owner->m_transform.GetPosition()) < data->owner->GetWeapon().weapon->range
		&& glm::dot(glm::normalize(data->targetShip->m_transform.GetPosition() - data->owner->m_transform.GetPosition()), glm::normalize(data->owner->m_transform.GetForwardVector())) > 0.75)
	{
		data->owner->GetWeapon().Shoot(data->owningTeam, glm::normalize(data->targetShip->m_transform.GetPosition() - data->owner->m_transform.GetPosition()), data->owner->m_transform.GetPosition());
		return BTNodeResult::eBTSuccess;
	}
}

BTNodeResult BTShipLookingAtTarget::Evaluate(void* ptr) const
{
	if (ptr == nullptr)
		return BTNodeResult::eBTFail;

	ShipAIData* data = static_cast<ShipAIData*>(ptr);

	if (glm::dot(glm::normalize(data->targetShip->m_transform.GetPosition() - data->owner->m_transform.GetPosition()), data->owner->m_transform.GetForwardVector()) > 0.75f)
	{
		return BTNodeResult::eBTSuccess;
	}
	return BTNodeResult::eBTFail;
}

BTNodeResult BTShipRotateToTarget::Evaluate(void* ptr) const
{
	if (ptr == nullptr)
		return BTNodeResult::eBTFail;

	ShipAIData* data = static_cast<ShipAIData*>(ptr);

	glm::vec3 toTargetShip = data->targetShip->m_transform.GetPosition() - data->owner->m_transform.GetPosition();

	bool rightSide = glm::dot(
		glm::cross(glm::normalize(data->owner->m_transform.GetForwardVector()), glm::vec3(0, 1, 0)),
		glm::normalize(toTargetShip)
	) > 0 ? true : false;

	float angleBetween = glm::acos(glm::dot(glm::normalize(toTargetShip), data->owner->m_transform.GetForwardVector()));

	data->owner->m_transform.SetRotation(data->owner->m_transform.GetRotation() * glm::quat(glm::vec3(0, rightSide ? angleBetween : -angleBetween, 0)));
}