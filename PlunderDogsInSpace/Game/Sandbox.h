#pragma once
#include "../Core/Application.h"
#include "PlayerController.h"
#include "Team.h"

class Planet
{
public:
	Planet() : m_transform(), orbitSpeed(10.f)
	{}
	Transform m_transform;
	float orbitSpeed;
};

class Sandbox : public Application
{
public:
	Sandbox();
	virtual bool Init() override final;
	virtual void Update() override final;
	virtual void Render() override final;
	virtual void EndOfFrame() override final;
	Team& GetTeam(const int TeamNum)
	{
		if (TeamNum == 2)
		{
			return team2;
		}
		return team1;
	}
protected:
	PlayerController m_controller;
	Team team1;
	Team team2;
	Planet testPlanet;
};

