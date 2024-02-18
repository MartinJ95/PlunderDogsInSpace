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
	void ReturnEmitter(Emitter&& Emitter)
	{
		m_cleaningEmitters.emplace_back(std::move(Emitter));
	}
	void ClaimEmitter(Emitter& OutEmitter)
	{
		m_emitters.GetNextObject(OutEmitter);
		//Emitter e = m_emitters.GetNextObject();
		//return std::move(e);
	}
protected:
	PlayerController m_controller;
	Team team1;
	Team team2;
	Planet testPlanet;
	ObjectPool<Emitter> m_emitters;
	std::vector<Emitter> m_cleaningEmitters;
};

