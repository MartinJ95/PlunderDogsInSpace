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

	if (glm::length(data->targetShip->m_transform.GetPosition() - data->owner->m_transform.GetPosition()) < data->owner->GetWeapon().weapon->range)
	{
		data->owner->GetWeapon().Shoot(data->owningTeam, glm::normalize(data->targetShip->m_transform.GetPosition() - data->owner->m_transform.GetPosition()), data->owner->m_transform.GetPosition());
		return BTNodeResult::eBTSuccess;
	}
}