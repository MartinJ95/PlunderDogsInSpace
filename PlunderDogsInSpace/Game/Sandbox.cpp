#include "Sandbox.h"
#include "PlayerController.h"
#include "../Physics/Collisions.h"

Sandbox::Sandbox() : Application(), team1(glm::vec3(0.f, 0.f, 0.f), false), team2(glm::vec3(0.f, 0.f, -25.f), true), testPlanet(), m_emitters(200, std::move(Emitter(900))), testingGrid(-20.f, 20.f)
{
}

bool Sandbox::Init()
{
    team1.Init(&team2);
    team2.Init(&team1);
    testPlanet.m_transform.SetPosition(glm::vec3(0.f, -10.f, -5.f));
    for (int i = 0; i < SmallGridTileAmount; i++)
    {
        for (int j = 0; j < SmallGridTileAmount; j++)
        {
            TestTile& t = testingGrid.GetTile(TileIndex(i, j));
            t.t.SetPosition(testingGrid.GetPointOnTile(TileIndex(i, j)));
            t.t.SetScale(glm::vec3(0.1f));
            t.t.EndFrame();
            t.t.CheckModelXForm();
        }
    }
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
    for (int i = 0; i < 10; i++)
    {
        if (!m_graphics.ShaderExist(i))
            continue;

        m_graphics.GetShader(i).SetRender3D(m_graphics.GetCamera());

        switch (i)
        {
        case 0:
            if (!m_graphics.GetShader(0).m_uniforms.UniformLocationExists(m_graphics.GetShader(0).GetID(), "model_xform"))
                break;
            m_graphics.Render(3, 0, true, testPlanet.m_transform.GetModelXform());
            for (Emitter& e : m_cleaningEmitters)
            {
                e.Render();
            }
            for (int i = 0; i < SmallGridTileAmount; i++)
            {
                for (int j = 0; j < SmallGridTileAmount; j++)
                {
                    m_graphics.Render(3, 0, true, testingGrid.GetTile(TileIndex(i, j)).t.GetModelXform());
                }
            }
            std::cout << "particle num" << "\n" << "---------------" << "\n" << Emitter::ParticleNum << "\n";
            break;
        default:
            break;
        }
        team1.Render(i);
        team2.Render(i);
        m_controller.Render(i);

    }

    //m_graphics.GetShader(0).SetRender3D(m_graphics.GetCamera());
    
    /*m_graphics.Render(0, 0, true, glm::mat4(
        10.f, 0.f, 0.f, 0.f,
        0.f, 10.f, 0.f, 0.f,
        0.f, 0.f, 10.f, 0.f,
        0.f, 0.f, 0.f, 10.f
    ));*/
    
    
    //DefaultGraphics& graphics = ServiceLocator::GetGraphics();
    //graphics.GetShader(0).SetRender3D(graphics.GetCamera());
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
