#pragma once
#include "../Core/Application.h"
#include "PlayerController.h"
#include "Team.h"

struct ParticleProperties
{
	float lifeTime = static_cast<float>((rand() % 5) + 5);
	glm::vec3 velocityChange = glm::vec3(static_cast<float>((rand() % 10) - 5), static_cast<float>((rand() % 10) - 5), static_cast<float>(rand() % 10) - 5);
	glm::vec3 startPosition;
	glm::vec3 color;
};

struct Particle
{
	ParticleProperties properties;
	float currentLifetime;
	Transform transform;
};

class Emitter
{
public:
	Emitter(const unsigned int MaxParticles) : maxParticles(MaxParticles), m_particles(), currentParticles(0), emitTime(1.f), emitTimer(0.f)
	{
		m_particles.reserve(maxParticles);
	}
	void Update(const Transform& AttachedTransform, const glm::vec3& ParticleColor)
	{
		emitTimer += ServiceLocator::GetTimeService().deltaTime;

		while (emitTimer > emitTime)
		{
			emitTimer -= emitTime;

			ParticleProperties properties;
			properties.startPosition = AttachedTransform.GetPosition();
			properties.color = glm::vec3(1, 0, 0);
			Emit(properties);
		}

		for (int i = 0; i < currentParticles; i++)
		{
			m_particles[i].transform.Move(m_particles[i].properties.velocityChange);
			m_particles[i].transform.CheckModelXForm();
			m_particles[i].currentLifetime += ServiceLocator::GetTimeService().deltaTime;
		}

		for (int i = currentParticles; i > 0; i--)
		{
			if (m_particles[i].currentLifetime > m_particles[i].properties.lifeTime)
			{
				std::swap(m_particles[i], m_particles[currentParticles--]);
			}
		}
	}
	void Render()
	{
		DefaultGraphics graphics = ServiceLocator::GetGraphics();

		graphics.GetShader(0).SetRender3D(graphics.GetCamera());

		for (int i = 0; i < currentParticles; i++)
		{
			graphics.Render(3, 0, true, m_particles[i].transform.GetModelXform());
		}
	}
	void Emit(const ParticleProperties& properties)
	{
		m_particles[++currentParticles].currentLifetime = 0;
		m_particles[currentParticles].transform.SetPosition(properties.startPosition);
		m_particles[currentParticles].transform.CheckModelXForm();
		m_particles[currentParticles].properties = properties;
	}
	const unsigned int maxParticles;
private:
	std::vector<Particle> m_particles;
	unsigned int currentParticles;
	float emitTime;
	float emitTimer;
};

class Planet
{
public:
	Planet() : m_transform(), orbitSpeed(10.f)
	{}
	Transform m_transform;
	float orbitSpeed;
};

class Sandbox : public Application
{
public:
	Sandbox();
	virtual bool Init() override final;
	virtual void Update() override final;
	virtual void Render() override final;
	virtual void EndOfFrame() override final;
	Team& GetTeam(const int TeamNum)
	{
		if (TeamNum == 2)
		{
			return team2;
		}
		return team1;
	}
protected:
	PlayerController m_controller;
	Team team1;
	Team team2;
	Planet testPlanet;
};

