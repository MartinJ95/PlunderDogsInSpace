#pragma once
#include "../Core/ServiceLocator.h"
#include "../AI/BehaviourTree.h"

class Team;

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
protected:
	ShipAIData m_shipAIData;
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
	friend class Ship;
protected:
	glm::vec3 m_startPosition;
	Team* m_otherTeamRef;
	std::vector<Ship> m_ships;
	bool isAI;
};

