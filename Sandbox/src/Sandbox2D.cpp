#include"Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <chrono>


Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f/720.0f), m_ParticleSystem(1000)
{

}
void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");

	m_SpriteSheet = Hazel::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

	m_TextureTable = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet,{8,2},{128,128});
	m_TextureStairs = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1,11 }, { 128,128 });

	
	// Particle props init here
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 2.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
	
	} 
void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();

} 

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	//Timer timer("Sandbox2D::OnUpdate", [&](ProfileResult profileRestult) { m_ProfileResults.push_back(profileRestult); });
	HZ_PROFILE_FUNCTION();
	
	
	// Update
	{
		HZ_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}
	

	// Render
	Hazel::Renderer2D::ResetStats();

	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor({ 0.1, 0.13, 0.13, 1 });
		Hazel::RenderCommand::Clear();
	}
#if 0
	{
		static float rotation = 0.0f;
		rotation += ts * 40.0f;
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

		//
		Hazel::Renderer2D::DrawRotatedQuad({ 1.0f,0.0f }, { 0.8f,0.8f }, glm::radians(-45.0f), { 0.8f,0.2f,0.3f,1.0f });
		Hazel::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 0.8f,0.8f }, { 0.8f,0.2f,0.3f,1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.5f,-0.5f }, { 0.5f,0.75f }, { 0.2f,0.3f,0.8f,1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f},{20.0f,20.0f},m_CheckerboardTexture, 10.0f);
		Hazel::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 1.0f }, { 1.0f,1.0f }, glm::radians(rotation),m_CheckerboardTexture, 20.0f);
		Hazel::Renderer2D::EndScene();

		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = {(x+5.0f)/10.0f,0.4f,(y+5.0f)/10.0f,0.5f};
				Hazel::Renderer2D::DrawQuad({x,y},{0.45f,0.45f},color);
			}
		}
		Hazel::Renderer2D::EndScene();
	}
#endif


	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Hazel::Input::GetMousePosition();
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();//关于从被鼠标点击的屏幕坐标转换到世界空间位置
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		//m_Particle.LifeTime = 5;
		for (int i = 0; i < 1; i++)
			m_ParticleSystem.Emit(m_Particle);
	}


	

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	//Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	//Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, 1.0f }, { 1.0f,1.0f }, m_TextureStairs);
	//Hazel::Renderer2D::DrawQuad({ 1.0f, 0.0f, 1.0f }, { 1.0f,1.0f }, m_TextureTable);
	//Hazel::Renderer2D::EndScene();


	//Hazel::Renderer2D::EndScene();
}
void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();
	ImGui::Begin("Settings");

	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d",stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	//ImGui::Text("Camera viewport: %d\t%d",m_CameraController.GetCamera())


	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));



	ImGui::End();
}
void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}