#pragma once
#include <vector>
#include "../Physics/Collisions.h"
#include <iostream>

class Ship;

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

enum class ClickButton
{
	eNone,
	eLeft,
	eRight
};

enum class ClickType
{
	eNoClick,
	eSingleClick,
	eDoubleClick,
	eHeldDown
};

constexpr float ClickResetTime{ 0.1f };

struct ClickManager
{
	ClickManager() : initialClickLocation(glm::vec3(0.f)), finalClickLocation(glm::vec3(0.f)), currentTime(0.f), currentClickType(ClickType::eNoClick), currentClickButton(ClickButton::eNone), lock(false), checked(false)
	{}
	void OnClick(const glm::vec2& ClickLocation, const ClickButton PressedButton);
	void OnRelease(const glm::vec2& ReleaseLocation, const ClickButton PressedButton);
	void Update();
	void FrameEnd();
	void ResetClick();
	ClickType GetCurrentClickType();
	bool GetBeingheld() const
	{
		return lock;
	}
	bool GetChecked() const
	{
		return checked;
	}
	glm::vec2 initialClickLocation;
	glm::vec2 finalClickLocation;
	float currentTime;
	ClickButton currentClickButton;
private:
	ClickType currentClickType;
	bool lock;
	bool checked;
};

struct SelectionBox
{
	SelectionBox() : transform(), min(0.f), max(0.f), active(false)
	{}
	Transform transform;
	glm::vec3 min;
	glm::vec3 max;
	bool active;
};

class PlayerController
{
public:
	PlayerController();
	void Update();
	void Render(const unsigned int RenderPass = 0) const;
	void EndOfFrame();
protected:
	std::vector<ClickIndicator> m_clickIndicators;
	std::vector<Ship*> m_selectedShips;
	ClickManager m_clickManager;
	SelectionBox m_selectionBox;
};

