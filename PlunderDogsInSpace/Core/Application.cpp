#include "Application.h"

Application::Application()
{
}

bool Application::Init()
{
    if(!m_graphics.Init())
        return false;

    m_graphics.GetCamera().SetPos(glm::vec3(0.f, 25.f, 0.f));
    m_graphics.GetCamera().SetRotation(glm::quat(glm::vec3(glm::radians(-70.f), 0.f, 0.f)));

    return true;
}

void Application::Render()
{
    
}

void Application::PhysicsStep()
{
}

void Application::Update()
{
    
}

void Application::EndOfFrame()
{
    
}

void Application::Run()
{
    while (!m_graphics.ShouldWindowClose())
    {
        m_timeManager.TimeStep();
        m_graphics.Clear();
        Update();
        Render();
        PhysicsStep();
        m_graphics.Display();
        m_graphics.PollEvents();
        EndOfFrame();
    }
}
