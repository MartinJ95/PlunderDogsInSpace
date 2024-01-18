#include "Application.h"

Application::Application()
{
}

bool Application::Init()
{
    if(!m_graphics.Init())
        return false;

    m_graphics.GetCamera().SetPos(glm::vec3(0.f, 1.f, 0.f));
    m_graphics.GetCamera().SetRotation(glm::quat(glm::vec3(glm::radians(-70.f), 0.f, 0.f)));

    return true;
}

void Application::Render()
{
    m_graphics.GetShader(0).SetRender3D(m_graphics.GetCamera());
    m_graphics.Render(0, true, glm::mat4(
        10.f, 0.f, 0.f, 0.f,
        0.f, 10.f, 0.f, 0.f,
        0.f, 0.f, 10.f, 0.f,
        0.f, 0.f, 0.f, 10.f
    ));
}

void Application::PhysicsStep()
{
}

void Application::Update()
{
    glm::vec3 mouseMovement = glm::vec3(0.f);
    
    mouseMovement.x -= glfwGetKey(m_graphics.GetWindow(), GLFW_KEY_A);
    mouseMovement.x += glfwGetKey(m_graphics.GetWindow(), GLFW_KEY_D);
    mouseMovement.z += glfwGetKey(m_graphics.GetWindow(), GLFW_KEY_S);
    mouseMovement.z -= glfwGetKey(m_graphics.GetWindow(), GLFW_KEY_W);
    
    m_graphics.GetCamera().SetPos(m_graphics.GetCamera().GetPos() + (mouseMovement * m_timeManager.deltaTime));
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
