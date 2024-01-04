#include "Application.h"

Application::Application()
{
}

bool Application::Init()
{
    return false;
}

void Application::Run()
{
    while (!m_graphics.ShouldWindowClose())
    {
        m_graphics.Clear();
        m_graphics.Display();
    }
}
