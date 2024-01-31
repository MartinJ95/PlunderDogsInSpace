#include "Team.h"

Ship::Ship() : ModelID(1), m_transform(), m_body(), m_tree(std::move(new BTSequenceNode)), m_shipAIData(this), m_weapon(nullptr)
{
	m_tree.GetRoot()->AddChild(std::move(new BTShipFindTarget));
	m_tree.GetRoot()->AddChild(std::move(new BTShipSetMoveToLocation));
	m_tree.GetRoot()->AddChild(std::move(new BTShipMoveToLocation));
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

	m_tree.Evaluate(&m_shipAIData);
	/*
	if (m_shipAIData.targetShip == nullptr)
	{
		if (m_shipAIData.owningTeam->m_otherTeamRef->m_ships.size() == 0)
			return;
		float closest = 999999.f;

		Ship* closestShip = nullptr;

		for (Ship& s : m_shipAIData.owningTeam->m_otherTeamRef->m_ships)
		{
			float distance = glm::length(s.m_transform.GetPosition() - m_transform.GetPosition());
			if (distance < closest) 
			{
				closestShip = &s;
				closest = distance;
			}
		}
		m_shipAIData.targetShip = closestShip;
	}
	else
	{
		m_transform.Move(glm::normalize(m_shipAIData.targetShip->m_transform.GetPosition() - m_transform.GetPosition()) * ServiceLocator::GetTimeService().deltaTime);
	}*/
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
}

void Team::Render()
{
	for (Ship& s : m_ships)
	{
		s.Render();
	}
}

void Team::EndOfFrame()
{
	for (Ship& s : m_ships)
	{
		s.EndOfFrame();
	}
}

ShipAIData::ShipAIData(Ship* Owner) : owningTeam(nullptr), owner(Owner), targetShip(nullptr), targetLocation(glm::vec3(0.f))
{
}