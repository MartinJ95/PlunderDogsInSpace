#pragma once
#include "../Core/ServiceLocator.h"
#include "../AI/BehaviourTree.h"

class Team;
struct Ship;

struct Projectile
{
	float damage;
	float speed;
	int modelID;
};

struct ShotProjectile
{
	ShotProjectile(const Projectile& Projectile, const glm::vec3& Direction, float Lifetime = 10.f) : projectile(&Projectile), direction(Direction), currentLifetime(0.f), lifeTime(Lifetime), transform(), markedForDeletion(false)
	{
		transform.SetScale(glm::vec3(0.2f));
	}
	const Projectile* projectile;
	glm::vec3 direction;
	float currentLifetime;
	float lifeTime;
	Transform transform;
	bool markedForDeletion;
	void Update();
	void EndOfFrame();
};

constexpr Projectile DefaultGunProjectile{ 10.f, 1.f, 2 };
constexpr Projectile DefaultCannonProjectile{ 50.f, 0.5f, 2 };

struct Weapon
{
	const Projectile& projectile;
	float reloadSpeed;
	float range;
	int medelID;
};

constexpr Weapon DefaultGun{ DefaultGunProjectile, 1.f, 5.f, 1 };
constexpr Weapon DefaultCannon{ DefaultCannonProjectile, 5.f, 20.f, 1 };

struct EquippedWeapon
{
	EquippedWeapon() : weapon(nullptr), currentReload(0.f), hasShot(false)
	{}
	const Weapon* weapon;
	float currentReload;
	bool hasShot;
	void Update();
	void Shoot(Team* OwningTeam, const glm::vec3& Direction, const glm::vec3& Position);
};

struct ShipAIData
{
	ShipAIData(Ship* Owner);
	Team* owningTeam;
	Ship* owner;
	Ship* targetShip;
	glm::vec3 targetLocation;
};

struct Ship
{
	Ship();
	unsigned int ModelID;
	Transform m_transform;
	RigidBody m_body;
	BehaviourTree m_tree;
	void Init(Team* OwningTeam);
	void Update();
	void Render();
	void EndOfFrame();
	ShipAIData& GetShipAIData() 
	{
		return m_shipAIData;
	}
	EquippedWeapon& GetWeapon()
	{
		return m_weapon;
	}
	void SetWeapon(const Weapon& NewWeapon)
	{
		m_weapon.weapon = &NewWeapon;
	}
protected:
	ShipAIData m_shipAIData;
	EquippedWeapon m_weapon;
};

class Team
{
public:
	Team(glm::vec3 StartPos, bool IsAI);
public:
	void Init(Team* Other);
	void Update();
	void Render();
	void EndOfFrame();
	Team* GetOtherTeam()
	{
		return m_otherTeamRef;
	}
	std::vector<Ship>& GetShips()
	{
		return m_ships;
	}
	std::vector<ShotProjectile>& GetProjectiles()
	{
		return m_projectiles;
	}
	friend class Ship;
protected:
	glm::vec3 m_startPosition;
	Team* m_otherTeamRef;
	std::vector<Ship> m_ships;
	std::vector<ShotProjectile> m_projectiles;
	bool isAI;
};

class BTShipFindTarget : public BTDecoratorNode
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr)
	{
		if (ptr == nullptr)
			return BTNodeResult::eBTFail;

		ShipAIData* data = static_cast<ShipAIData*>(ptr);

		if (data->targetShip == nullptr || data->owningTeam->GetOtherTeam()->GetShips().size() != 0)
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
};

class BTShipSetMoveToLocation : public BTDecoratorNode
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr)
	{
		if (ptr == nullptr)
			return BTNodeResult::eBTFail;

		ShipAIData* data = static_cast<ShipAIData*>(ptr);

		if (data->targetShip == nullptr)
		{
			data->targetLocation = data->owner->m_transform.GetPosition();
			return BTNodeResult::eBTSuccess;
		}

		if (data->owner->GetWeapon().weapon == nullptr)
		{
			data->targetLocation = data->owner->m_transform.GetPosition();
			return BTNodeResult::eBTSuccess;
		}

		if (glm::length(data->targetShip->m_transform.GetPosition() - data->owner->m_transform.GetPosition()) < data->owner->GetWeapon().weapon->range)
		{
			data->targetLocation = data->owner->m_transform.GetPosition();
			return BTNodeResult::eBTSuccess;
		}

		data->targetLocation = data->targetShip->m_transform.GetPosition();
		return BTNodeResult::eBTSuccess;
	}
};

class BTShipMoveToLocation : public BTDecoratorNode
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) override
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
};

class BTShipShootAtTarget : public BTDecoratorNode
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr)
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
};