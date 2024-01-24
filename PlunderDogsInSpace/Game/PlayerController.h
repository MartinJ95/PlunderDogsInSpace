#pragma once
#include <vector>
#include "../Physics/Collisions.h"

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

class PlayerController
{
public:
	PlayerController();
	void Update();
	void Render() const;
	void EndOfFrame();
protected:
	std::vector<ClickIndicator> m_clickIndicators;
};

