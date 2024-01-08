#include "Application.h"

Application::Application()
{
}

bool Application::Init()
{
    return m_graphics.Init();
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
        m_graphics.Clear();
        Update();
        Render();
        PhysicsStep();
        m_graphics.Display();
        m_graphics.PollEvents();
        EndOfFrame();
    }
}
