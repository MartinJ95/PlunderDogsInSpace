#pragma once
#include "../Core/ServiceLocator.h"
#include "../AI/BehaviourTree.h"

struct ship
{
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
	void Update();
	void Render();
	void EndOfFrame();
};

