#pragma once

#include "Hazel.h"

#include "ParticleSystem.h"

#define PARTICLES 0
#define MAPTILES 0 

class Sandbox2D :public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;	

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;

	

private:
	Hazel::OrthographicCameraController m_CameraController;

	
	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;
#if MAPTILES
	Hazel::Ref<Hazel::Texture2D> m_SpriteSheet;
	Hazel::Ref<Hazel::SubTexture2D> m_TextureStairs;
	Hazel::Ref<Hazel::SubTexture2D> m_TextureTable;

	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, Hazel::Ref<Hazel::SubTexture2D>> s_TextureMap;
#endif
	glm::vec4 m_SquareColor = { 0.2f,0.3f,0.8f, 1.0f };

#if PARTICLES
	ParticleSystem m_ParticleSystem;//Á£×ÓÏµÍ³
	ParticleProps m_Particle;
#endif
	

};