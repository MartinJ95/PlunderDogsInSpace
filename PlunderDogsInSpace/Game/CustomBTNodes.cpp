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

	if (data->targetShip == nullptr)
		return BTNodeResult::eBTFail;

	if (glm::dot(glm::normalize(data->targetShip->m_transform.GetPosition() - data->owner->m_transform.GetPosition()), data->owner->m_transform.GetForwardVector()) > 0.95f)
	{
		return BTNodeResult::eBTSuccess;
	}
	return BTNodeResult::eBTFail;
}

inline void RotateTowardsPoint(const glm::vec3& TargetPoint, Transform& ShipTransform, const float RotationSpeed = 1.f)
{
	glm::vec3 toTargetPoint = TargetPoint - ShipTransform.GetPosition();

	bool rightSide = glm::dot(
		glm::normalize(glm::cross(glm::normalize(ShipTransform.GetForwardVector()), glm::vec3(0, 1, 0))),
		glm::normalize(toTargetPoint)
	) > 0 ? true : false;

	float angleBetween = glm::acos(glm::dot(glm::normalize(toTargetPoint), ShipTransform.GetForwardVector()));

	glm::quat newRot = ShipTransform.GetRotation() * glm::quat(glm::vec3(0, rightSide ? angleBetween : -angleBetween, 0));

	if (newRot != newRot)
		return;

	ShipTransform.SetRotation(newRot);
}

BTNodeResult BTShipRotateToTarget::Evaluate(void* ptr) const
{
	if (ptr == nullptr)
		return BTNodeResult::eBTFail;

	ShipAIData* data = static_cast<ShipAIData*>(ptr);

	if (data->targetShip == nullptr)
		return BTNodeResult::eBTFail;


	RotateTowardsPoint(data->targetShip->m_transform.GetPosition(), data->owner->m_transform);
}

BTNodeResult BTShipRotateToTargetLocation::Evaluate(void* ptr) const
{
	if (ptr == nullptr)
		return BTNodeResult::eBTFail;

	ShipAIData* data = static_cast<ShipAIData*>(ptr);

	glm::vec3 toTargetLocation = data->targetLocation - data->owner->m_transform.GetPosition();

	if (glm::length(toTargetLocation) < 2.f)
		return BTNodeResult::eBTFail;
	
	RotateTowardsPoint(data->targetLocation, data->owner->m_transform);
}

BTNodeResult BTSucceed::Evaluate(void* ptr) const
{
	return BTNodeResult::eBTSuccess;
}

BTNodeResult BTHasGotTarget::Evaluate(void* ptr) const
{
	if (ptr == nullptr)
		return BTNodeResult::eBTFail;

	ShipAIData* data = static_cast<ShipAIData*>(ptr);

	if (data->targetShip == nullptr)
		return BTNodeResult::eBTFail;

	return BTNodeResult::eBTSuccess;
}

BTNodeResult BTSetPhysicsMoveForward::Evaluate(void* ptr) const
{
	if (ptr == nullptr)
		return BTNodeResult::eBTFail;
	ShipAIData* data = static_cast<ShipAIData*>(ptr);

	if (data->targetShip == nullptr)
		return BTNodeResult::eBTFail;

	Ship* ship = data->owner;

	ship->m_body.SetForce(ship->m_transform.GetForwardVector());
}

BTNodeResult BTSetPhysicsMoveBackword::Evaluate(void* ptr) const
{
	if (ptr == nullptr)
		return BTNodeResult::eBTFail;
	ShipAIData* data = static_cast<ShipAIData*>(ptr);

	if (data->targetShip == nullptr)
		return BTNodeResult::eBTFail;

	Ship* ship = data->owner;

	ship->m_body.SetForce(-ship->m_transform.GetForwardVector());
}

BTNodeResult BTApplyPhysics::Evaluate(void* ptr) const
{
	if (ptr == nullptr)
		return BTNodeResult::eBTFail;
	ShipAIData* data = static_cast<ShipAIData*>(ptr);

	if (data->targetShip == nullptr)
		return BTNodeResult::eBTFail;

	Ship* ship = data->owner;

	ship->m_body.ApplyPhysics(ServiceLocator::GetTimeService().deltaTime, ship->m_transform);
}

void AIFindingTarget::ConstructState(std::unordered_map<std::string, BehaviourState>& States) const
{
	BehaviourTree tree(new BTSelectorNode);
	tree.GetRoot()->AddChild(new BTDecoratorNode(&BTHasGotTargetObj));
	tree.GetRoot()->AddChild(new BTDecoratorNode(&BTShipFindTargetObj));

	std::vector<BehaviourTree> trees;

	trees.emplace_back(std::move(tree));

	BehaviourState state{ std::move(trees) };

	States.emplace(
		std::pair<std::string, BehaviourState>{
		std::string("Finding Target"),
			std::move(state)});
}

void ShootAtTarget::ConstructState(std::unordered_map<std::string, BehaviourState>& States) const
{
	BehaviourTree tree(new BTSequenceNode);
	tree.GetRoot()->AddChild(new BTDecoratorNode(&BTHasGotTargetObj));
	tree.GetRoot()->AddChild(new BTDecoratorNode(&BTShipLookingAtTargetObj));
	tree.GetRoot()->AddChild(new BTDecoratorNode(&BTShipShootAtTargetObj));

	std::vector<BehaviourTree> trees;

	trees.emplace_back(std::move(tree));

	BehaviourState state{ std::move(trees) };

	States.emplace(
		std::pair<std::string, BehaviourState>{
		std::string("shooting at target"),
			std::move(state)});
}

void MoveToTargetLocation::ConstructState(std::unordered_map<std::string, BehaviourState>& States) const
{
	BehaviourTree tree(new BTSelectorNode);
	tree.GetRoot()->AddChild(new BTDecoratorNode(&BTShipMoveToLocationObj));

	BehaviourTree rotationTree(new BTSelectorNode);
	rotationTree.GetRoot()->AddChild(new BTDecoratorNode(&BTShipRotateToTargetObj));

	std::vector<BehaviourTree> trees;
	trees.reserve(2);

	trees.emplace_back(std::move(tree));
	trees.emplace_back(std::move(rotationTree));

	BehaviourState state{ std::move(trees) };

	States.emplace(
		std::pair<std::string, BehaviourState>{
		std::string("moving to location"),
			std::move(state)
	}
	);
}

void FindTargetMoveLocation::ConstructState(std::unordered_map<std::string, BehaviourState>& States) const
{
	BehaviourTree tree(new BTSelectorNode);
	tree.GetRoot()->AddChild(new BTDecoratorNode(&BTShipSetMoveToLocationObj));

	std::vector<BehaviourTree> trees;

	trees.emplace_back(std::move(tree));

	BehaviourState state{ std::move(trees) };

	States.emplace(
		std::pair<std::string, BehaviourState>{
		std::string("set move to location"),
			std::move(state)});
}

void EmptyState::ConstructState(std::unordered_map<std::string, BehaviourState>& States) const
{
	BehaviourTree tree(new BTSelectorNode);
	
	std::vector<BehaviourTree> trees;

	trees.emplace_back(tree);

	BehaviourState state{ std::move(trees) };

	States.emplace(
		std::pair<std::string, BehaviourState>{
		std::string("none"),
			std::move(state)}
	);
}
