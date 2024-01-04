#include "Graphics.h"
#include "GLFW/glfw3.h"

Graphics::Graphics() : m_screenWidth(1280.f), m_screenHeight(720.f)
{
}

Graphics::Graphics(float ScreenWidth, float ScreenHeight) : m_screenWidth(ScreenWidth), m_screenHeight(m_screenHeight)
{
}

GLGraphics::GLGraphics() : Graphics()
{
}

GLGraphics::GLGraphics(float ScreenWidth, float ScreenHeight) : Graphics(ScreenWidth, ScreenHeight)
{
}

GLGraphics::~GLGraphics()
{
	glfwDestroyWindow(m_window);

	glfwTerminate();
}

void GLGraphics::Init()
{
	glfwInit();

	glfwCreateWindow(m_screenWidth, m_screenHeight, "testing", nullptr, nullptr);
}

void GLGraphics::SetCallbacks()
{
}

void GLGraphics::ResizeScreen()
{
}

bool GLGraphics::ShouldWindowClose()
{
	return glfwWindowShouldClose(m_window);
}

void GLGraphics::Clear()
{
	glClearColor(0.1f, 0.5f, 0.1f, 1.f);
}

void GLGraphics::Display()
{
	glfwSwapBuffers(m_window);
}
