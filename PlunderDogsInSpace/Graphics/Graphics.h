#pragma once

struct GLFWwindow;

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

class NullGraphics : public Graphics
{
public:
	NullGraphics();
public:
	virtual void Init() override final {}
	virtual void SetCallbacks() override final {}
	virtual void ResizeScreen() override final {}
	virtual bool ShouldWindowClose() override final { return true; }
	virtual void Clear() override final {}
	virtual void Display() override final {}
};

class GLGraphics : public Graphics
{
public:
	GLGraphics();
	GLGraphics(float ScreenWidth, float ScreenHeight);
	~GLGraphics();
public:
	virtual void Init() override final;
	virtual void SetCallbacks() override final;
	virtual void ResizeScreen() override final;
	virtual bool ShouldWindowClose() override final;
	virtual void Clear() override final;
	virtual void Display() override final;
public:
	GLFWwindow* m_window;
};