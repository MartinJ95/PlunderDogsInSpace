#include "Sandbox.h"
#include "../Physics/Collisions.h"

Sandbox::Sandbox() : Application(), team1(glm::vec3(0.f, 0.f, 0.f), false), team2(glm::vec3(0.f, 0.f, -25.f), true)
{
}

bool Sandbox::Init()
{
    team1.Init(&team2);
    team2.Init(&team1);
	return Application::Init();
}

void Sandbox::Update()
{
    m_controller.Update();
    team1.Update();
    team2.Update();
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
    team1.Render();
    team2.Render();
	Application::Render();
}

void Sandbox::EndOfFrame()
{
    m_controller.EndOfFrame();
    team1.EndOfFrame();
    team2.EndOfFrame();
	Application::EndOfFrame();
}
