#include "Sandbox.h"
#include "../Physics/Collisions.h"

Sandbox::Sandbox() : Application()
{
}

bool Sandbox::Init()
{
	return Application::Init();
}

void Sandbox::Update()
{
    m_controller.Update();
	Application::Update();
}

void Sandbox::Render()
{
    m_graphics.GetShader(0).SetRender3D(m_graphics.GetCamera());
    m_graphics.Render(0, 0, true, glm::mat4(
        10.f, 0.f, 0.f, 0.f,
        0.f, 10.f, 0.f, 0.f,
        0.f, 0.f, 10.f, 0.f,
        0.f, 0.f, 0.f, 10.f
    ));
    m_controller.Render();
	Application::Render();
}

void Sandbox::EndOfFrame()
{
    m_controller.EndOfFrame();
	Application::EndOfFrame();
}
