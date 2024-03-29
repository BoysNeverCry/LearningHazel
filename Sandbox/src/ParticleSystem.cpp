#include "ParticleSystem.h"


#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>


#include <random>

class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};


std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

ParticleSystem::ParticleSystem(uint32_t maxParticles)
	:m_PoolIndex(maxParticles-1)
{
	m_ParticlePool.resize(maxParticles);
}

void ParticleSystem::OnUpdate(Hazel::Timestep ts)
{
	
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}
		
		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.01f * ts;
	}
	
}

void ParticleSystem::OnRender(Hazel::OrthographicCamera& camera)
{
	//int count = 0;
	//glUseProgram(m_ParticleShader->GetRendererID());
	//glUniformMatrix4fv(m_ParticleShaderViewProj, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix()));
	Hazel::Renderer2D::BeginScene(camera);
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		// Fade away particles
		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		//color.a = color.a * life;

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		// Render
		glm::vec3 position = { particle.Position.x, particle.Position.y,1.0};
		Hazel::Renderer2D::DrawRotatedQuad(position, {size,size}, particle.Rotation, color);
		//count++;


	}
	//HZ_CORE_INFO("Particle counts: {0}", count);
	Hazel::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

	// Color
	particle.ColorBegin = glm::vec4(0.6+ Random::Float()/2.5f, 0.8+ Random::Float()/5, Random::Float(), Random::Float());//particleProps.ColorBegin;
	particle.ColorEnd = glm::vec4(Random::Float(), Random::Float(), Random::Float(), Random::Float());//particleProps.ColorEnd;

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;
	
	if(m_PoolIndex==0) m_PoolIndex = m_ParticlePool.size()-1;
	else m_PoolIndex--;
	//m_PoolIndex = (m_PoolIndex) % m_ParticlePool.size();
	//HZ_CORE_INFO("Pool Index: {0} / {1}", m_PoolIndex, m_ParticlePool.size());
}