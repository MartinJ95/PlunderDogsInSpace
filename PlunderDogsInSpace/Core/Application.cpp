#include "Application.h"

Application::Application()
{
}

bool Application::Init()
{
    return m_graphics.Init();
}

void Application::Run()
{
    while (!m_graphics.ShouldWindowClose())
    {
        m_graphics.Clear();
        m_graphics.Display();
        m_graphics.PollEvents();
    }
}
