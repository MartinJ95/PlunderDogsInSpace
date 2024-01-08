#pragma once
#include "../Graphics/Graphics.h"
#include <chrono>
#ifdef OpenGL
typedef GLGraphics DefaultGraphics;
#else
typedef NullGraphics DefaultGraphics;
#endif // !OpenGL

struct TimeManager
{
	TimeManager() : lastUpdate(std::chrono::steady_clock::now()), deltaTime(0.f)
	{}
	void TimeStep()
	{
		auto now = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.f;
		lastUpdate = now;
	}
	std::chrono::steady_clock::time_point lastUpdate;
	float deltaTime;
};

class Application
{
public:
	Application();
	virtual bool Init();
	virtual void Render();
	virtual void PhysicsStep();
	virtual void Update();
	virtual void EndOfFrame();
	void Run();
protected:
	DefaultGraphics m_graphics;
	TimeManager m_timeManager;
};
