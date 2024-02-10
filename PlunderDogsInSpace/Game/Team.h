#pragma once
#include "../Core/ServiceLocator.h"
#include "../AI/BehaviourTree.h"
#include "../Physics/Collisions.h"

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
	ShotProjectile(const Projectile& Projectile, const glm::vec3& Direction, float Lifetime = 50.f) : projectile(&Projectile), direction(Direction), currentLifetime(0.f), lifeTime(Lifetime), transform(), collider(0.2f), markedForDeletion(false)
	{
		transform.SetScale(glm::vec3(0.2f));
	}
	const Projectile* projectile;
	glm::vec3 direction;
	float currentLifetime;
	float lifeTime;
	Transform transform;
	SphereCollider collider;
	bool markedForDeletion;
	void Update();
	void EndOfFrame();
};

constexpr Projectile DefaultGunProjectile{ 10.f, 1.f, 3 };
constexpr Projectile DefaultCannonProjectile{ 50.f, 0.5f, 3 };

struct Weapon
{
	const Projectile& projectile;
	float reloadSpeed;
	float range;
	int medelID;
};

constexpr Weapon DefaultGun{ DefaultGunProjectile, 1.f, 5.f, 2 };
constexpr Weapon DefaultCannon{ DefaultCannonProjectile, 5.f, 20.f, 2 };

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
	ShipAIData(const ShipAIData& other) : owningTeam(other.owningTeam), owner(other.owner), targetShip(other.targetShip), targetLocation(other.targetLocation)
	{}
	void operator=(const ShipAIData& other)
	{
		owningTeam = other.owningTeam;
		owner = other.owner;
		targetShip = other.targetShip;
		targetLocation = other.targetLocation;
	}
	Team* owningTeam;
	Ship* owner;
	Ship* targetShip;
	glm::vec3 targetLocation;
};

struct ShipClass
{
	float health;
	float speed;
	const Weapon& weapon;
};

constexpr ShipClass Fighter{ 50.f, 2.f, DefaultGun };
constexpr ShipClass Cruiser{ 150.f, 1.f, DefaultCannon };

constexpr glm::vec3 HealthbarScale = glm::vec3(1.f, 1.f, 0.2f);

constexpr float HealthBarHideTime{ 2.5f };

struct Ship
{
	Ship();
	Ship(const Ship& other);
	Ship(Ship&& other);
	void operator=(const Ship& other)
	{
		ModelID = other.ModelID;
		m_transform = other.m_transform;
		m_healthbarTransform = other.m_healthbarTransform;
		m_healthbarTransform.SetScale(HealthbarScale);
		m_body = other.m_body;
		m_collider = other.m_collider;
		m_tree = other.m_tree;
		currentHealth = other.currentHealth;
		m_shipAIData = other.m_shipAIData;
		m_shipAIData.owner = this;
		m_class = other.m_class;
		m_weapon = other.m_weapon;
		markedForDeletion = other.markedForDeletion;
		selected = other.selected;
	}
	void operator=(Ship&& other)
	{
		ModelID = other.ModelID;
		m_transform = other.m_transform;
		m_healthbarTransform = other.m_healthbarTransform;
		m_healthbarTransform.SetScale(HealthbarScale);
		m_body = other.m_body;
		m_collider = other.m_collider;
		m_tree = std::move(other.m_tree);
		currentHealth = other.currentHealth;
		m_shipAIData = other.m_shipAIData;
		m_shipAIData.owner = this;
		m_class = other.m_class;
		m_weapon = other.m_weapon;
		markedForDeletion = other.markedForDeletion;
		selected = other.selected;
	}
	unsigned int ModelID;
	Transform m_transform;
	Transform m_healthbarTransform;
	RigidBody m_body;
	SphereCollider m_collider;
	BehaviourTree m_tree;
	float currentHealth;
	void Init(Team* OwningTeam);
	void Update();
	void Render();
	void EndOfFrame();
	ShipAIData& GetShipAIData() 
	{
		return m_shipAIData;
	}
	const ShipClass* GetClass() const
	{
		return m_class;
	}
	void SetClass(const ShipClass& Class)
	{
		m_class = &Class;
		currentHealth = Class.health;
		SetWeapon(m_class->weapon);
	}
	EquippedWeapon& GetWeapon()
	{
		return m_weapon;
	}
	void SetWeapon(const Weapon& NewWeapon)
	{
		m_weapon.weapon = &NewWeapon;
	}
	void TakeDamage(const float DamageRecieved)
	{
		timeLastAffected = ServiceLocator::GetTimeService().totalTime;
		currentHealth -= DamageRecieved;
		if (currentHealth <= 0)
		{
			markedForDeletion = true;
		}
	}
	bool SelectShip(std::vector<Ship*>& SelectedShips)
	{
		bool isInVector = false;
		for (Ship* s : SelectedShips)
		{
			if (s == this)
			{
				isInVector = true;
				break;
			}
		}
		if (!isInVector)
		{
			SelectedShips.emplace_back(this);
			selected = true;
			return true;
		}
	}
	bool SelectShip(const glm::vec3& ClickPos, std::vector<Ship*>& SelectedShips)
	{
		SphereCollider s(1.f);
		CollisionData data;

		data.t = &m_transform;
		Transform clickTransform;
		clickTransform.SetPosition(ClickPos);
		clickTransform.EndFrame();

		data.otherT = &clickTransform;

		SphereToSphere(m_collider, s, data);
		
		if (data.hasHit)
		{
			return SelectShip(SelectedShips);
			/*bool isInVector = false;
			for (Ship* s : SelectedShips)
			{
				if (s == this)
				{
					isInVector = true;
					break;
				}
			}
			if (!isInVector)
			{
				SelectedShips.emplace_back(this);
				selected = true;
				return true;
			}*/
		}
		return false;
	}
	void Deselect()
	{
		selected = false;
	}
	void MoveShip(const glm::vec3& ClickPos)
	{
		m_shipAIData.targetLocation = ClickPos;
	}
	bool IsMarkedForDelection() const
	{
		return markedForDeletion;
	}
protected:
	ShipAIData m_shipAIData;
	const ShipClass* m_class;
	EquippedWeapon m_weapon;
	float timeLastAffected;
	bool markedForDeletion;
	bool selected;
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

struct BTShipFindTarget : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override
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
};

struct BTShipSetMoveToLocation : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override
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

struct BTShipMoveToLocation : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override
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

struct BTShipShootAtTarget : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override
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

constexpr BTShipFindTarget BTShipFindTargetObj;
constexpr BTShipSetMoveToLocation BTShipSetMoveToLocationObj;
constexpr BTShipMoveToLocation BTShipMoveToLocationObj;
constexpr BTShipShootAtTarget BTShipShootAtTargetObj;