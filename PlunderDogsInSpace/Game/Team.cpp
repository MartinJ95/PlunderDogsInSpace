#include "Team.h"
#include "Sandbox.h"
#include <stack>

int Emitter::ParticleNum{0};

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
		Sandbox* s = (Sandbox*)ServiceLocator::GetMainService();
		hasShot = true;
		Emitter e(900, false);
		s->ClaimEmitter(e);
		//ShotProjectile test{ weapon->projectile, Direction, std::move(e) };
		OwningTeam->GetProjectiles().emplace_back(std::move(ShotProjectile(weapon->projectile, Direction, std::move(e))));
		OwningTeam->GetProjectiles().back().transform.SetPosition(Position);
		OwningTeam->GetProjectiles().back().transform.EndFrame();
	}
}

Ship::Ship() : ModelID(2), m_transform(), m_healthbarTransform(), m_body(), m_collider(1.f), m_tree(std::move(new BTSequenceNode)), m_behaviourStates({nullptr}),
currentHealth(0.f), markedForDeletion(false), m_class(nullptr), m_shipAIData(this), m_weapon(), selected(false), timeLastAffected(ServiceLocator::GetTimeService().totalTime)
{
	m_healthbarTransform.SetScale(HealthbarScale);
}

Ship::Ship(const Ship& other) : ModelID(other.ModelID), m_transform(other.m_transform), m_healthbarTransform(other.m_healthbarTransform), m_body(other.m_body),
m_collider(other.m_collider), m_tree(other.m_tree), m_behaviourStates(other.m_behaviourStates), currentHealth(other.currentHealth),
markedForDeletion(other.markedForDeletion), m_shipAIData(other.m_shipAIData), m_class(other.m_class), m_weapon(other.m_weapon), selected(false)
{
	m_shipAIData.owner = this;
	m_healthbarTransform.SetScale(HealthbarScale);
}

Ship::Ship(Ship&& other) : ModelID(other.ModelID), m_transform(other.m_transform), m_healthbarTransform(other.m_healthbarTransform), m_body(other.m_body),
m_collider(other.m_collider), m_tree(std::move(other.m_tree)), m_behaviourStates(std::move(other.m_behaviourStates)), currentHealth(other.currentHealth),
markedForDeletion(other.markedForDeletion), m_shipAIData(other.m_shipAIData), m_class(other.m_class), m_weapon(other.m_weapon), selected(false)
{
	m_shipAIData.owner = this;
	m_healthbarTransform.SetScale(HealthbarScale);
}

void Ship::Init(Team* OwningTeam)
{
	m_shipAIData.owningTeam = OwningTeam;
	m_behaviourStates.targeting = &BehaviourState::GetState("Finding Target");
	if (OwningTeam->isAI)
	{
		m_behaviourStates.movementDecisionMaking = &BehaviourState::GetState("set move to location");
	}
	//m_behaviourStates.movement = &BehaviourState::GetState("moving to location");
	m_behaviourStates.movement = &BehaviourState::GetState("fighter move to location");
	m_behaviourStates.engaging = &BehaviourState::GetState("shooting at target");

	/*m_tree.GetRoot()->AddChild(std::move(new BTDecoratorNode(&BTShipFindTargetObj)));
	if (OwningTeam->isAI)
	{
		m_tree.GetRoot()->AddChild(std::move(new BTDecoratorNode(&BTShipSetMoveToLocationObj)));
	}
	m_tree.GetRoot()->AddChild(std::move(new BTSelectorNode()));
	BTSelectorNode* SN = static_cast<BTSelectorNode*>(m_tree.GetRoot()->GetChildren().back());
	SN->AddChild(std::move(new BTSelectorNode()));
	BTSelectorNode* SN2 = static_cast<BTSelectorNode*>(SN->GetChildren().back());
	SN2->AddChild(std::move(new BTDecoratorNode(&BTShipLookingAtTargetObj)));
	SN2->AddChild(std::move(new BTDecoratorNode(&BTShipRotateToTargetObj)));
	SN->AddChild(std::move(new BTDecoratorNode(&BTSucceedObj)));
	//m_tree.GetRoot()->AddChild(std::move(new BTDecoratorNode(&BTShipSetMoveToLocationObj)));
	m_tree.GetRoot()->AddChild(std::move(new BTDecoratorNode(&BTShipMoveToLocationObj)));
	m_tree.GetRoot()->AddChild(std::move(new BTDecoratorNode(&BTShipShootAtTargetObj)));
	*/
}

void Ship::Update()
{
	m_tree.Evaluate(&m_shipAIData);
	m_behaviourStates.EvaluateStates(m_shipAIData);

	m_body.ApplyPhysics(ServiceLocator::GetTimeService().deltaTime, m_transform);
	m_transform.CheckModelXForm();

	m_healthbarTransform.SetPosition(m_transform.GetPosition() + (-ServiceLocator::GetGraphics().GetCamera().GetCameraUp() * 3.f));
	m_healthbarTransform.CheckModelXForm();

	m_weapon.Update();

	//m_tree.Evaluate(&m_shipAIData);
}

void Ship::Render(const unsigned int RenderPass)
{
	DefaultGraphics& graphics = ServiceLocator::GetGraphics();

	//graphics.GetShader(0).SetRender3D(graphics.GetCamera());

	switch (RenderPass)
	{
	case 0:
		graphics.Render(ModelID, 0, true, m_transform.GetModelXform());
		break;
	case 1:
		break;
	case 2:
		if ((ServiceLocator::GetTimeService().totalTime - timeLastAffected) < HealthBarHideTime)
		{
			graphics.GetShader(2).SetRender3D(graphics.GetCamera());

			graphics.GetShader(2).m_uniforms.SetFloat(graphics.GetShader(2).GetID(), "health", currentHealth);

			graphics.GetShader(2).m_uniforms.SetFloat(graphics.GetShader(2).GetID(), "totalHealth", m_class->health);

			graphics.Render(0, 2, true, m_healthbarTransform.GetModelXform());
		}
		break;
	case 3:
		if (selected)
		{
			graphics.GetShader(3).SetRender3D(graphics.GetCamera());
			glm::vec3 v(0, 0, 1);

			v = v * glm::quat(glm::vec3(0, glm::radians(ServiceLocator::GetTimeService().totalTime * 15.f), 0));

			graphics.GetShader(3).m_uniforms.SetVec3(graphics.GetShader(3).GetID(), "rotatedTimeVec", v);

			graphics.Render(1, 3, true, m_transform.GetModelXform());
		}
		break;
	default:
		break;
	}	
	//graphics.GetShader(0).SetRender3D(graphics.GetCamera());
}

void Ship::EndOfFrame()
{
	m_transform.EndFrame();
	m_healthbarTransform.EndFrame();
}

void Ship::MoveShip(const glm::vec3& ClickPos)
{
	Sandbox* s = (Sandbox*)ServiceLocator::GetMainService();
	m_shipAIData.targetLocation = s->GetTestingGrid().GetPointOnTile(ClickPos);
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
		m_ships.back().GetShipAIData().targetLocation = m_ships.back().m_transform.GetPosition();
		m_ships.back().SetClass((i % 2 == 0 ? Fighter : Cruiser));
		m_ships.back().m_body.SetMass(10.f);
		//m_ships.back().SetWeapon((i % 2 == 0 ? DefaultGun : DefaultCannon));
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
				s.TakeDamage(p.projectile->damage);
			}
		}
	}
}

void Team::Render(const unsigned int RenderPass)
{
	for (Ship& s : m_ships)
	{
		s.Render(RenderPass);
	}

	switch (RenderPass)
	{
	case 0:
		for (ShotProjectile& p : m_projectiles)
		{
			p.transform.CheckModelXForm();
			//DefaultGraphics& graphics = ServiceLocator::GetGraphics();
			//graphics.Render(p.projectile->modelID, 0, true, p.transform.GetModelXform());
			p.emitter.Render();
		}
		break;
	default:
		break;
	}
	//DefaultGraphics& graphics = ServiceLocator::GetGraphics();
	//graphics.GetShader(0).SetRender3D(graphics.GetCamera());
	
	//std::cout << ParticleNum * sizeof(Particle) << std::endl;
	//std::cout << m_projectiles.size() * 900 * sizeof(Particle) << std::endl;
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

	for (int i = 0; i < m_ships.size(); i++)
	{
		if (m_ships[i].IsMarkedForDelection())
		{
			toDelete.emplace(i);
		}
	}
	if (!toDelete.empty())
	{
		for (Ship& s : m_ships)
		{
			s.GetShipAIData().targetShip = nullptr;
		}
		for (Ship& s : m_otherTeamRef->GetShips())
		{
			s.GetShipAIData().targetShip = nullptr;
		}
	}
	while (!toDelete.empty())
	{
		int index = toDelete.top();
		toDelete.pop();
		m_ships.erase(m_ships.begin() + index);
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
	emitter.Update(transform, glm::vec3(1, 0, 0));
}

void ShotProjectile::EndOfFrame()
{
	transform.EndFrame();
}