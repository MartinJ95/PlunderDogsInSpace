#include "Graphics.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwCreateWindow(m_screenWidth, m_screenHeight, "testing", nullptr, nullptr);

	glfwMakeContextCurrent(m_window);
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

NullGraphics::NullGraphics() : Graphics()
{
}
