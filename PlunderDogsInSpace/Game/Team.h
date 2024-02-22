#pragma once
#include "../Core/ServiceLocator.h"
#include "../AI/BehaviourTree.h"
#include "../Physics/Collisions.h"
#include <queue>

class Team;
struct Ship;

template <typename T>
class ObjectPool
{
public:
	ObjectPool(const unsigned int PoolSize, const T&& initial) : m_objects(), size(PoolSize)
	{
		for (int i = 0; i < PoolSize; i++)
		{
			m_objects.emplace(T(initial));
		}
	}
	void ReleaseObject(T&& object)
	{
		m_objects.emplace(std::move(object));
	}
	void GetNextObject(T& OutEmitter)
	{
		//T obj = std::move(m_objects.front());
		OutEmitter = std::move(m_objects.front());
		m_objects.pop();
		//return std::move(obj);
	}
private:
	std::queue<T> m_objects;
	const unsigned int size;
};

constexpr float velocityChangeModifier = 0.0005f;

struct ParticleProperties
{
	float lifeTime = static_cast<float>((rand() % 2) + 1);
	glm::vec3 velocityChange = glm::vec3(static_cast<float>((rand() % 10) - 5)* velocityChangeModifier, static_cast<float>((rand() % 10) - 5)* velocityChangeModifier, (static_cast<float>(rand() % 10) - 5)* velocityChangeModifier);
	glm::vec3 scale = glm::vec3(0.1f);
	glm::vec3 startPosition;
	glm::vec3 color;
};

struct Particle
{
	ParticleProperties properties;
	float currentLifetime;
	Transform transform;
};

class Emitter
{
public:
	Emitter(const unsigned int MaxParticles, bool Initialise = true) : maxParticles(MaxParticles), m_particles(), currentParticles(0), emitTime(0.005f), emitTimer(0.f)
	{
		if (Initialise)
		{
			m_particles.resize(MaxParticles);
		}
		//m_particles.reserve(maxParticles);
	}
	Emitter(const Emitter& other) : maxParticles(other.maxParticles), m_particles(other.m_particles), currentParticles(other.currentParticles), emitTime(other.emitTime), emitTimer(other.emitTimer)
	{
		ParticleNum += currentParticles;
	}
	Emitter(Emitter&& other) : maxParticles(other.maxParticles), m_particles(std::move(other.m_particles)), currentParticles(other.currentParticles), emitTime(other.emitTime), emitTimer(other.emitTimer)
	{
		other.currentParticles = 0;
		other.m_particles.clear();
	}
	~Emitter()
	{
		ParticleNum -= currentParticles;
	}
	void operator=(const Emitter& other)
	{
		m_particles = other.m_particles;
		currentParticles = other.currentParticles;
		emitTime = other.emitTime;
		emitTimer = other.emitTimer;
	}
	void operator=(Emitter&& other)
	{
		m_particles = std::move(other.m_particles);
		currentParticles = other.currentParticles;
		emitTime = other.emitTime;
		emitTimer = other.emitTimer;
		other.currentParticles = 0;
		other.m_particles.clear();
	}
	void Update(const Transform& AttachedTransform, const glm::vec3& ParticleColor)
	{
		Update();

		while (emitTimer > emitTime)
		{
			emitTimer -= emitTime;

			ParticleProperties properties;
			properties.startPosition = AttachedTransform.GetPosition();
			properties.color = glm::vec3(1, 0, 0);
			Emit(properties);
		}
	}
	void Update()
	{
		emitTimer += ServiceLocator::GetTimeService().deltaTime;

		for (int i = 0; i < currentParticles; i++)
		{
			m_particles[i].transform.Move(m_particles[i].properties.velocityChange);
			m_particles[i].transform.EndFrame();
			m_particles[i].transform.CheckModelXForm();
			m_particles[i].currentLifetime += ServiceLocator::GetTimeService().deltaTime;
		}

		for (int i = currentParticles; i > 0; i--)
		{
			if (m_particles[i].currentLifetime > m_particles[i].properties.lifeTime)
			{
				std::swap(m_particles[i], m_particles[currentParticles--]);
				ParticleNum--;
			}
		}
	}
	void Render()
	{
		DefaultGraphics& graphics = ServiceLocator::GetGraphics();

		//graphics.GetShader(0).SetRender3D(graphics.GetCamera());

		if (!graphics.DoesModelExist(3))
			return;

		for (int i = 0; i < currentParticles; i++)
		{
			graphics.Render(3, 0, true, m_particles[i].transform.GetModelXform());
		}
	}
	void Emit(const ParticleProperties& properties)
	{
		m_particles[++currentParticles].currentLifetime = 0;
		m_particles[currentParticles].transform.SetPosition(properties.startPosition);
		m_particles[currentParticles].transform.SetScale(properties.scale);
		m_particles[currentParticles].transform.EndFrame();
		m_particles[currentParticles].transform.CheckModelXForm();
		m_particles[currentParticles].properties = properties;
		ParticleNum++;
	}
	const unsigned int GetCurrentParticles() const
	{
		return currentParticles;
	}
	const unsigned int maxParticles;
	static int ParticleNum;
private:
	std::vector<Particle> m_particles;
	unsigned int currentParticles;
	float emitTime;
	float emitTimer;
};

struct Projectile
{
	float damage;
	float speed;
	int modelID;
};

struct ShotProjectile
{
	ShotProjectile(const Projectile& Projectile, const glm::vec3& Direction, Emitter&& Emitter, float Lifetime = 50.f) : projectile(&Projectile), direction(Direction), currentLifetime(0.f), lifeTime(Lifetime), transform(), collider(0.2f), emitter(std::move(Emitter)), markedForDeletion(false)
	{
		transform.SetScale(glm::vec3(0.2f));
	}
	ShotProjectile(const ShotProjectile& other) : projectile(other.projectile), direction(other.direction), currentLifetime(other.currentLifetime), lifeTime(other.lifeTime), transform(other.transform),
		collider(other.collider), emitter(other.emitter), markedForDeletion(other.markedForDeletion)
	{
	}
	ShotProjectile(ShotProjectile&& other) : projectile(other.projectile), direction(other.direction), currentLifetime(other.currentLifetime), lifeTime(other.lifeTime),  emitter(std::move(other.emitter)), transform(other.transform),
		collider(other.collider), markedForDeletion(other.markedForDeletion)
	{
	}
	~ShotProjectile()
	{}
	void operator=(const ShotProjectile& other)
	{
		projectile = other.projectile;
		direction = other.direction;
		currentLifetime = other.currentLifetime;
		lifeTime = other.lifeTime;
		transform = other.transform;
		collider = other.collider;
		emitter = other.emitter;
		markedForDeletion = other.markedForDeletion;
	}
	void operator=(ShotProjectile&& other)
	{
		projectile = other.projectile;
		direction = other.direction;
		currentLifetime = other.currentLifetime;
		lifeTime = other.lifeTime;
		transform = other.transform;
		collider = other.collider;
		emitter = std::move(other.emitter);
		markedForDeletion = other.markedForDeletion;
	}
	const Projectile* projectile;
	glm::vec3 direction;
	float currentLifetime;
	float lifeTime;
	Emitter emitter;
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
	void Render(const unsigned int RenderPass = 0);
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
	void Render(const unsigned int RenderPass = 0);
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