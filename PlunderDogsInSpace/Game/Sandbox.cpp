#include "Sandbox.h"
#include "PlayerController.h"
#include "../Physics/Collisions.h"

Sandbox::Sandbox() : Application(), team1(glm::vec3(0.f, 0.f, 0.f), false), team2(glm::vec3(0.f, 0.f, -25.f), true), testPlanet(), m_emitters(200, std::move(Emitter(900)))
{
}

bool Sandbox::Init()
{
    team1.Init(&team2);
    team2.Init(&team1);
    testPlanet.m_transform.SetPosition(glm::vec3(0.f, -10.f, -5.f));
	return Application::Init();
}

void Sandbox::Update()
{
    m_controller.Update();
    team1.Update();
    team2.Update();
    testPlanet.m_transform.SetRotation(testPlanet.m_transform.GetRotation() * glm::quat(glm::vec3(0.f, glm::radians(testPlanet.orbitSpeed * ServiceLocator::GetTimeService().deltaTime), 0.f)));
    testPlanet.m_transform.CheckModelXForm();
    for (int i = m_cleaningEmitters.size(); i < 0; i--)
    {
        m_cleaningEmitters[i].Update();
        if (m_cleaningEmitters[i].GetCurrentParticles() == 0)
        {
            m_emitters.ReleaseObject(std::move(m_cleaningEmitters[i]));
            m_cleaningEmitters.erase(m_cleaningEmitters.begin() + i);
        }
    }
    Application::Update();
}

void Sandbox::Render()
{
    m_graphics.GetShader(0).SetRender3D(m_graphics.GetCamera());
    m_graphics.Render(3, 0, true, testPlanet.m_transform.GetModelXform());
    /*m_graphics.Render(0, 0, true, glm::mat4(
        10.f, 0.f, 0.f, 0.f,
        0.f, 10.f, 0.f, 0.f,
        0.f, 0.f, 10.f, 0.f,
        0.f, 0.f, 0.f, 10.f
    ));*/
    team1.Render();
    team2.Render();
    m_controller.Render();
    for (Emitter& e : m_cleaningEmitters)
    {
        e.Render();
    }
	Application::Render();
}

void Sandbox::EndOfFrame()
{
    m_controller.EndOfFrame();
    team1.EndOfFrame();
    team2.EndOfFrame();
    testPlanet.m_transform.EndFrame();
	Application::EndOfFrame();
}
