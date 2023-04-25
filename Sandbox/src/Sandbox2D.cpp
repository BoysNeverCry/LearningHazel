#include"Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <chrono>


Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f/720.0f)
{

}
void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	
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
	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor({ 0.1, 0.13, 0.13, 1 });
		Hazel::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 40.0f;
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

		//
		Hazel::Renderer2D::DrawRotatedQuad({ -1.0f,0.0f }, { 0.8f,0.8f }, glm::radians(70.0f), { 0.8f,0.2f,0.3f,1.0f });
		Hazel::Renderer2D::DrawRotatedQuad({ 0.5f,-0.2f }, { 0.8f,1.3f }, -40.0f, { 0.2f,0.7f,0.3f,1.0f });
		Hazel::Renderer2D::DrawRotatedQuad({ 0.0f,-0.0f,-0.2f }, { 10.0f,10.0f },glm::radians(80.0f), m_CheckerboardTexture, 10.0f,glm::vec4(0.8f,0.9f,0.8f,1.0f));
		Hazel::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 0.8f,0.8f }, { 0.8f,0.2f,0.3f,1.0f });
		Hazel::Renderer2D::DrawQuad({ -0.5f,-0.5f }, { 0.5f,0.75f }, { 0.2f,0.3f,0.8f,1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f},{10.0f,10.0f},m_CheckerboardTexture, 10.0f);
		Hazel::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f,1.0f }, rotation,m_CheckerboardTexture, 20.0f);
	}


	Hazel::Renderer2D::EndScene();
}
void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));



	ImGui::End();
}
void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}