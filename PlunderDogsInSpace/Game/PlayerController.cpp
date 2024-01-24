#include "PlayerController.h"
#include "Sandbox.h"
#include "../Core/ServiceLocator.h"

PlayerController::PlayerController() : m_clickIndicators()
{
}

void PlayerController::Update()
{
    glm::vec3 mouseMovement = glm::vec3(0.f);

    TimeManager& TimeService = ServiceLocator::GetTimeService();
    DefaultGraphics& graphics = ServiceLocator::GetGraphics();
    
    mouseMovement.x -= glfwGetKey(graphics.GetWindow(), GLFW_KEY_A);
    mouseMovement.x += glfwGetKey(graphics.GetWindow(), GLFW_KEY_D);
    mouseMovement.z += glfwGetKey(graphics.GetWindow(), GLFW_KEY_S);
    mouseMovement.z -= glfwGetKey(graphics.GetWindow(), GLFW_KEY_W);

    graphics.GetCamera().SetPos(graphics.GetCamera().GetPos() + (mouseMovement * TimeService.deltaTime));

    if (glfwGetMouseButton(graphics.GetWindow(), GLFW_MOUSE_BUTTON_1))
    {
        Ray ray;
        ray.origin = graphics.GetCamera().GetPos();
        //ray.line = glm::normalize(m_graphics.GetCamera().GetViewDir()) * 10.f;
        ray.line = glm::normalize(graphics.GetCamera().GetViewDirOnScreen(graphics.GetScreenDimensions(), graphics.GetMouseLocation())) * 100.f;
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
    for (ClickIndicator& c : m_clickIndicators)
    {
        c.m_currentTime += TimeService.deltaTime;
        if (c.m_currentTime > c.m_lifetime)
        {
            c.m_markedForDeletion = true;
            continue;
        }
        c.m_transform.CheckModelXForm();
    }
}

void PlayerController::Render() const
{
    DefaultGraphics& graphics = ServiceLocator::GetGraphics();
    TimeManager& timeService = ServiceLocator::GetTimeService();
    graphics.GetShader(1).SetRender3D(graphics.GetCamera());

    for (int i = 0; i < m_clickIndicators.size(); i++)
    {
        if (m_clickIndicators[i].m_markedForDeletion)
            continue;
        graphics.GetShader(1).m_uniforms.SetFloat(graphics.GetShader(1).ID, "currentTime", m_clickIndicators[i].m_currentTime);
        graphics.GetShader(1).m_uniforms.SetFloat(graphics.GetShader(1).ID, "totalTime", m_clickIndicators[i].m_lifetime);
        //m_graphics.GetShader(1).m_uniforms.SetVec3(m_graphics.GetShader(1).ID, "cameraPosition", m_graphics.GetCamera().GetPos());
        graphics.Render(0, 1, true, m_clickIndicators[i].m_transform.GetModelXform());
    }
}

void PlayerController::EndOfFrame()
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
}
