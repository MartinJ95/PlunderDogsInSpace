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
    glm::vec3 mouseMovement = glm::vec3(0.f);

    mouseMovement.x -= glfwGetKey(m_graphics.GetWindow(), GLFW_KEY_A);
    mouseMovement.x += glfwGetKey(m_graphics.GetWindow(), GLFW_KEY_D);
    mouseMovement.z += glfwGetKey(m_graphics.GetWindow(), GLFW_KEY_S);
    mouseMovement.z -= glfwGetKey(m_graphics.GetWindow(), GLFW_KEY_W);

    m_graphics.GetCamera().SetPos(m_graphics.GetCamera().GetPos() + (mouseMovement * m_timeManager.deltaTime));

    if (glfwGetMouseButton(m_graphics.GetWindow(), GLFW_MOUSE_BUTTON_1))
    {
        Ray ray;
        ray.origin = m_graphics.GetCamera().GetPos();
        //ray.line = glm::normalize(m_graphics.GetCamera().GetViewDir()) * 10.f;
        ray.line = glm::normalize(m_graphics.GetCamera().GetViewDirOnScreen(m_graphics.GetScreenDimensions(), m_graphics.GetMouseLocation())) * 100.f;
        //std::cout << "x: " << ray.line.x << "\n" << "y: " << ray.line.y << "\n" << "z: " << ray.line.z << "\n" << "\n";
        //std::cout << "x: " << ray.origin.x << "\n" << "y: " << ray.origin.y << "\n" << "z: " << ray.origin.z << "\n" << "\n";

        PlaneCollider plane;
        plane.planeNormal = glm::vec3(0.f, 1.f, 0.f);

        CollisionData data;

        RayToPlane(ray, plane, data);

        if (data.hasHit)
        {
            m_clickIndicators.emplace_back();
            m_clickIndicators.back().m_transform.SetPosition(data.pointOfCollision);
            //m_entities.back().m_body.GetTransform().SetPosition(ray.origin + (ray.line * 0.1f));
        }
        //std::cout << data;
    }
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

    m_graphics.GetShader(1).SetRender3D(m_graphics.GetCamera());

    for (int i = 0; i < m_clickIndicators.size(); i++)
    {
        m_clickIndicators[i].m_currentTime += m_timeManager.deltaTime;
        if (m_clickIndicators[i].m_currentTime > m_clickIndicators[i].m_lifetime)
        {
            m_clickIndicators[i].m_markedForDeletion = true;
            continue;
        }
        m_graphics.GetShader(1).m_uniforms.SetFloat(m_graphics.GetShader(1).ID, "currentTime", m_clickIndicators[i].m_currentTime);
        m_graphics.GetShader(1).m_uniforms.SetFloat(m_graphics.GetShader(1).ID, "totalTime", m_clickIndicators[i].m_lifetime);
        //m_graphics.GetShader(1).m_uniforms.SetVec3(m_graphics.GetShader(1).ID, "cameraPosition", m_graphics.GetCamera().GetPos());
        m_graphics.Render(0, 1, true, m_clickIndicators[i].m_transform.GetModelXform());
    }
	Application::Render();
}

void Sandbox::EndOfFrame()
{
    int deletedIndicators = 0;

    for (int i = 0; i < m_clickIndicators.size(); i++)
    {
        if (m_clickIndicators[i].m_markedForDeletion)
        {
            if (m_clickIndicators.size() > 1)
            {
                std::swap(m_clickIndicators[i], m_clickIndicators[m_clickIndicators.size() - ++deletedIndicators]);
            }
            else
            {
                deletedIndicators++;
            }
        }
    }
    int initialSize = m_clickIndicators.size();
    for (int i = 0; i < deletedIndicators; i++)
    {
        m_clickIndicators.erase(m_clickIndicators.begin() + (initialSize - (i + 1)));
    }
    for (int i = 0; i < m_clickIndicators.size(); i++)
    {
        m_clickIndicators[i].m_transform.EndFrame();
    }
	Application::EndOfFrame();
}
