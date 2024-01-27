#pragma once
#include "../Core/Application.h"
#include "PlayerController.h"
#include "Team.h"

class Sandbox : public Application
{
public:
	Sandbox();
	virtual bool Init() override final;
	virtual void Update() override final;
	virtual void Render() override final;
	virtual void EndOfFrame() override final;
protected:
	PlayerController m_controller;
	Team team1;
	Team team2;
};

