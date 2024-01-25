#pragma once
#include "../Core/ServiceLocator.h"
#include "../AI/BehaviourTree.h"

struct Ship
{
	Ship();
	unsigned int ModelID;
	Transform m_transform;
	RigidBody m_body;
	BehaviourTree m_tree;
	void Update();
	void Render();
	void EndOfFrame();
};

class Team
{
public:
	Team(glm::vec3 StartPos);
public:
	void Init(const Team* Other);
	void Update();
	void Render();
	void EndOfFrame();
protected:
	glm::vec3 m_startPosition;
	Team* m_otherTeamRef;
	std::vector<Ship> m_ships;
};

