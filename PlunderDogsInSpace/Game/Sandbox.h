#pragma once
#include "../Core/Application.h"

struct ClickIndicator
{
	ClickIndicator() : m_transform(), m_meshID(0), m_currentTime(0.f), m_lifetime(0.5f), m_markedForDeletion(false)
	{}
	Transform m_transform;
	unsigned int m_meshID;
	float m_currentTime;
	float m_lifetime;
	bool m_markedForDeletion;
};

class Sandbox : public Application
{
public:
	Sandbox();
	virtual bool Init() override final;
	virtual void Update() override final;
	virtual void Render() override final;
	virtual void EndOfFrame() override final;
protected:
	std::vector<ClickIndicator> m_clickIndicators;
};

