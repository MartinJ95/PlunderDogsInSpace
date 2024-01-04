#pragma once
class Graphics
{
public:
	Graphics();
	Graphics(float ScreenWidth, float ScreenHeight);
public:
	virtual void Init() = 0;
	virtual void SetCallbacks() = 0;
	virtual void ResizeScreen() = 0;
	virtual void Clear() = 0;
	virtual void Display() = 0;
protected:
	float m_screenWidth, m_screenHeight;
};

