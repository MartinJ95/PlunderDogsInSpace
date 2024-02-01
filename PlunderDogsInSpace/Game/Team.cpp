#include "Team.h"
#include <stack>

void EquippedWeapon::Update()
{
	if (hasShot)
	{
		currentReload += ServiceLocator::GetTimeService().deltaTime;
		if (currentReload >= weapon->reloadSpeed)
		{
			currentReload = 0.f;
			hasShot = false;
		}
	}
}

void EquippedWeapon::Shoot(Team* OwningTeam, const glm::vec3& Direction, const glm::vec3& Position)
{
	if (!hasShot)
	{
		hasShot = true;
		OwningTeam->GetProjectiles().emplace_back(ShotProjectile(weapon->projectile, Direction));
		OwningTeam->GetProjectiles().back().transform.SetPosition(Position);
	}
}

Ship::Ship() : ModelID(1), m_transform(), m_body(), m_collider(1.f), m_tree(std::move(new BTSequenceNode)), m_shipAIData(this), m_weapon()
{
	m_tree.GetRoot()->AddChild(std::move(new BTShipFindTarget));
	m_tree.GetRoot()->AddChild(std::move(new BTShipSetMoveToLocation));
	m_tree.GetRoot()->AddChild(std::move(new BTShipMoveToLocation));
	m_tree.GetRoot()->AddChild(std::move(new BTShipShootAtTarget));
}

void Ship::Init(Team* OwningTeam)
{
	m_shipAIData.owningTeam = OwningTeam;
}

void Ship::Update()
{
	m_tree.Evaluate();

	m_body.ApplyPhysics(ServiceLocator::GetTimeService().deltaTime, m_transform);
	m_transform.CheckModelXForm();

	m_weapon.Update();

	m_tree.Evaluate(&m_shipAIData);
}

void Ship::Render()
{
	DefaultGraphics& graphics = ServiceLocator::GetGraphics();

	graphics.GetShader(0).SetRender3D(graphics.GetCamera());

	graphics.Render(ModelID, 0, true, m_transform.GetModelXform());
}

void Ship::EndOfFrame()
{
	m_transform.EndFrame();
}

Team::Team(glm::vec3 StartPos, bool IsAI) : m_startPosition(StartPos), m_otherTeamRef(nullptr), m_ships(), isAI(IsAI)
{
}

void Team::Init(Team* Other)
{
	m_ships.reserve(3);

	m_otherTeamRef = Other;
	glm::vec3 toOtherTeam = m_otherTeamRef->m_startPosition - m_startPosition;
	glm::vec3 sideVec = glm::normalize(glm::cross(glm::normalize(toOtherTeam), glm::vec3(0.f, 1.f, 0.f)));

	float shipSpacing = 5.f;

	glm::vec3 startShipPos = m_startPosition - sideVec * shipSpacing;
	for (int i = 0; i < 3; i++)
	{
		m_ships.emplace_back();
		m_ships.back().m_transform.SetPosition(startShipPos + sideVec * (shipSpacing * i));
		m_ships.back().Init(this);
		m_ships.back().EndOfFrame();
		m_ships.back().SetWeapon((i % 2 == 0 ? DefaultGun : DefaultCannon));
	}
}

void Team::Update()
{
	for (Ship& s : m_ships)
	{
		s.Update();
	}
	for (ShotProjectile& p : m_projectiles)
	{
		p.Update();
		for (Ship& s : m_otherTeamRef->m_ships)
		{
			CollisionData data;
			data.t = &p.transform;
			data.otherT = &s.m_transform;
			SphereToSphere(p.collider, s.m_collider, data);
			if (data.hasHit)
			{
				p.markedForDeletion = true;
			}
		}
	}
}

void Team::Render()
{
	for (Ship& s : m_ships)
	{
		s.Render();
	}
	for (ShotProjectile& p : m_projectiles)
	{
		p.transform.CheckModelXForm();
		DefaultGraphics& graphics = ServiceLocator::GetGraphics();
		graphics.Render(p.projectile->modelID, 0, true, p.transform.GetModelXform());
	}
}

void Team::EndOfFrame()
{
	for (Ship& s : m_ships)
	{
		s.EndOfFrame();
	}
	for (ShotProjectile& p : m_projectiles)
	{
		p.EndOfFrame();
	}
	
	std::stack<int> toDelete;

	for (int i = 0; i < m_projectiles.size(); i++)
	{
		if (m_projectiles[i].markedForDeletion)
		{
			toDelete.emplace(i);
		}
	}
	while (!toDelete.empty())
	{
		int index = toDelete.top();
		toDelete.pop();
		m_projectiles.erase(m_projectiles.begin() + index);
	}
}

ShipAIData::ShipAIData(Ship* Owner) : owningTeam(nullptr), owner(Owner), targetShip(nullptr), targetLocation(glm::vec3(0.f))
{
}

void ShotProjectile::Update()
{
	currentLifetime += ServiceLocator::GetTimeService().deltaTime;
	if (currentLifetime >= lifeTime)
	{
		markedForDeletion = true;
	}
	transform.Move(direction * ServiceLocator::GetTimeService().deltaTime);
}

void ShotProjectile::EndOfFrame()
{
	transform.EndFrame();
}
