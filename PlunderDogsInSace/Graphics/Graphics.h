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
	virtual bool ShouldWindowClose() = 0;
	virtual void Clear() = 0;
	virtual void Display() = 0;
protected:
	float m_screenWidth, m_screenHeight;
};

class GLGraphics : public Graphics
{
public:
	GLGraphics();
	GLGraphics(float ScreenWidth, float ScreenHeight);
	~GLGraphics();
public:
	virtual void Init() override;
	virtual void SetCallbacks() override;
	virtual void ResizeScreen() override;
	virtual bool ShouldWindowClose() override;
	virtual void Clear() override;
	virtual void Display() override;
public:
	GLFWwindow* m_window;
};