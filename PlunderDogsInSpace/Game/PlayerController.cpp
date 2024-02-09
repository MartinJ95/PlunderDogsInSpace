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

    m_clickManager.Update();

    if (glfwGetMouseButton(graphics.GetWindow(), GLFW_MOUSE_BUTTON_1))
    {
        m_clickManager.OnClick(graphics.GetMouseLocation(), ClickButton::eLeft);
    }
    else
    {
        m_clickManager.OnRelease(graphics.GetMouseLocation(), ClickButton::eLeft);
    }

    if (glfwGetMouseButton(graphics.GetWindow(), GLFW_MOUSE_BUTTON_2))
    {
        m_clickManager.OnClick(graphics.GetMouseLocation(), ClickButton::eRight);
    }
    else
    {
        m_clickManager.OnRelease(graphics.GetMouseLocation(), ClickButton::eRight);
    }

    if (!m_clickManager.GetChecked())
    {
        ClickType click = m_clickManager.GetCurrentClickType();

        if (click == ClickType::eSingleClick || click == ClickType::eDoubleClick)
        {
            ClickButton button = m_clickManager.currentClickButton;

            Ray ray;
            ray.origin = graphics.GetCamera().GetPos();
            //ray.line = glm::normalize(m_graphics.GetCamera().GetViewDir()) * 10.f;
            ray.line = glm::normalize(graphics.GetCamera().GetViewDirOnScreen(graphics.GetScreenDimensions(), m_clickManager.initialClickLocation)) * 50.f;
            //std::cout << "x: " << ray.line.x << "\n" << "y: " << ray.line.y << "\n" << "z: " << ray.line.z << "\n" << "\n";
            //std::cout << "x: " << ray.origin.x << "\n" << "y: " << ray.origin.y << "\n" << "z: " << ray.origin.z << "\n" << "\n";

            PlaneCollider plane;
            plane.planeNormal = glm::vec3(0.f, 1.f, 0.f);

            CollisionData data;

            RayToPlane(ray, plane, data);

            if (data.hasHit)
            {
                //glm::vec3 clickPos = ray.origin + (ray.line * 1.f);
                glm::vec3 clickPos = data.pointOfCollision;

                m_clickIndicators.emplace_back();
                //m_clickIndicators.back().m_transform.SetPosition(data.pointOfCollision);
                m_clickIndicators.back().m_transform.SetPosition(clickPos);

                Sandbox* s = (Sandbox*)ServiceLocator::GetMainService();

                if (button == ClickButton::eLeft)
                {
                    bool selecting = false;

                    for (Ship& s : s->GetTeam(1).GetShips())
                    {
                        if (s.SelectShip(clickPos, m_selectedShips))
                        {
                            selecting = true;
                            break;
                        }
                    }
                    if (!selecting)
                    {
                        for (Ship* s : m_selectedShips)
                        {
                            s->Deselect();
                        }
                        m_selectedShips.clear();
                    }
                }
                else
                {
                    for (Ship* s : m_selectedShips)
                    {
                        s->MoveShip(clickPos);
                    }
                }
            }
            //std::cout << data;
        }
    }

    /*if (glfwGetMouseButton(graphics.GetWindow(), GLFW_MOUSE_BUTTON_1) || glfwGetMouseButton(graphics.GetWindow(), GLFW_MOUSE_BUTTON_2))
    {
        Ray ray;
        ray.origin = graphics.GetCamera().GetPos();
        //ray.line = glm::normalize(m_graphics.GetCamera().GetViewDir()) * 10.f;
        ray.line = glm::normalize(graphics.GetCamera().GetViewDirOnScreen(graphics.GetScreenDimensions(), graphics.GetMouseLocation())) * 50.f;
        //std::cout << "x: " << ray.line.x << "\n" << "y: " << ray.line.y << "\n" << "z: " << ray.line.z << "\n" << "\n";
        //std::cout << "x: " << ray.origin.x << "\n" << "y: " << ray.origin.y << "\n" << "z: " << ray.origin.z << "\n" << "\n";

        PlaneCollider plane;
        plane.planeNormal = glm::vec3(0.f, 1.f, 0.f);

        CollisionData data;

        RayToPlane(ray, plane, data);

        if (data.hasHit)
        {
            //glm::vec3 clickPos = ray.origin + (ray.line * 1.f);
            glm::vec3 clickPos = data.pointOfCollision;

            m_clickIndicators.emplace_back();
            //m_clickIndicators.back().m_transform.SetPosition(data.pointOfCollision);
            m_clickIndicators.back().m_transform.SetPosition(clickPos);

            Sandbox* s = (Sandbox*)ServiceLocator::GetMainService();

            if(glfwGetMouseButton(graphics.GetWindow(), GLFW_MOUSE_BUTTON_1))
            {
                bool selecting = false;

               for (Ship& s : s->GetTeam(1).GetShips())
               {
                   if (s.SelectShip(clickPos, m_selectedShips))
                   {
                       selecting = true;
                       break;
                   }
               }
               if (!selecting)
               {
                   for (Ship* s : m_selectedShips)
                   {
                       s->Deselect();
                   }
                   m_selectedShips.clear();
               }
            }
            else
            {
                for (Ship* s : m_selectedShips)
                {
                    s->MoveShip(clickPos);
                }
            }
        }
        //std::cout << data;
    }*/
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
    m_clickManager.FrameEnd();

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

void ClickManager::OnClick(const glm::vec2& ClickLocation, const ClickButton PressedButton)
{
    if (currentClickButton != ClickButton::eNone && PressedButton != currentClickButton)
    {
        if (!lock)
            ResetClick();
        return;
    }

    if (lock)
        return;

    if (currentClickButton == ClickButton::eNone)
    {
        currentClickButton = PressedButton;
        initialClickLocation = ClickLocation;
        currentClickType = ClickType::eSingleClick;
        lock = true;
        currentTime = 0.f;
        return;
    }
    if (currentClickType == ClickType::eSingleClick)
    {
        lock = true;
        currentClickType = ClickType::eDoubleClick;
        currentTime = 0.f;
    }
    finalClickLocation = ClickLocation;
}

void ClickManager::OnRelease(const glm::vec2& ReleaseLocation, const ClickButton PressedButton)
{
    if (currentClickButton != PressedButton || currentClickButton == ClickButton::eNone)
        return;

    lock = false;
    //currentTime = 0.f;
}

void ClickManager::Update()
{

    if (currentClickType == ClickType::eNoClick || currentClickButton == ClickButton::eNone)
        return;
    std::cout << static_cast<int>(currentClickType) << std::endl;

    currentTime += ServiceLocator::GetTimeService().deltaTime;

    std::cout << currentTime << std::endl;
}

void ClickManager::FrameEnd()
{
    if (currentClickType == ClickType::eNoClick || currentClickButton == ClickButton::eNone)
        return;

    if (currentTime < ClickResetTime)
        return;

    if (!lock)
        return ResetClick();
    
    currentClickType = ClickType::eHeldDown;
}

void ClickManager::ResetClick()
{
    initialClickLocation = glm::vec2(0.f);
    finalClickLocation = glm::vec2(0.f);
    currentTime = 0.f;
    currentClickType = ClickType::eNoClick;
    currentClickButton = ClickButton::eNone;
    checked = false;
}

ClickType ClickManager::GetCurrentClickType()
{
    if (currentClickType == ClickType::eSingleClick && currentTime < ClickResetTime)
        return ClickType::eNoClick;

    if (currentClickType == ClickType::eSingleClick || currentClickType == ClickType::eDoubleClick)
    {
        checked = true;
    }

    return currentClickType;
}
