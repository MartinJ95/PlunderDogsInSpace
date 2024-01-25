#include "Team.h"

Ship::Ship() : ModelID(1), m_transform(), m_body(), m_tree(std::move(new BTSelectorNode))
{
}

void Ship::Update()
{
	m_tree.Evaluate();

	m_body.ApplyPhysics(ServiceLocator::GetTimeService().deltaTime, m_transform);
	m_transform.CheckModelXForm();
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

Team::Team(glm::vec3 StartPos) : m_startPosition(StartPos), m_otherTeamRef(nullptr), m_ships()
{
}

void Team::Init(const Team* Other)
{

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
		s.Update();
	}
}

void Team::EndOfFrame()
{
	for (Ship& s : m_ships)
	{
		s.EndOfFrame();
	}
}