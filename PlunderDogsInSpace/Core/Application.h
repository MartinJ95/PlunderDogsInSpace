#pragma once
#include "../Graphics/Graphics.h"
#ifdef OpenGL
typedef GLGraphics DefaultGraphics;
#else
typedef NullGraphics DefaultGraphics;
#endif // !OpenGL

class Application
{
public:
	Application();
	virtual bool Init();
	virtual void Run();
protected:
	DefaultGraphics m_graphics;
};

