
#include"EditorLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <chrono>

namespace Hazel{

	#if MAPTILES
	static const uint32_t s_MapWidth = 24;
	//static const uint32_t s_MapHeight = 24;
	static const char* s_MapTiles = 
	"WWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWWWDDDDDDDWWWWWWWWWWW"
	"WWWWDDDDDDDDDDDWWWWWWWWW"
	"WWDDDDDDDDDDDDDDDDDWWWWW"
	"WWWWDDDDDDDyyDDDDWWWWWWW"
	"WWWWDDDyyyyyyDDDWWWWWWWW"
	"WWWWWdDDDDyyyDDDDDWWWWWW"
	"WWWWWWWDDDDDDDDDWWWWWWWW"
	"WWWWWWDDDDDDDDDWWWWWWWWW"
	"WWWWDDDDDDDDDDWWWWWWWWWW"
	"WWWWWWWWWWWWWWWDDDDDDDDy"
	"WWWWWWWWWWWWDDDDDDDDyyyy"
	"WWWWWWWWWDDDDDDDDDyyyyyy"
	"WWWWWWWWWWWDDDDDDDDDDDyy"
	"WWWWWWWWWWWWWWWDDDDDDDDD"
	"WWWWWWWWWWWWWWWDDDDDDDDD"
	"WWWWWWWWWWWWWWWWDDDDDDDD"
	"WWWWWWWWWWWWWWWWWWWWDDDD";
	#endif

	EditorLayer::EditorLayer()
		:Layer("EditorLayer"), m_CameraController(1280.0f/720.0f)
	{

	}
	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");

		Hazel::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Hazel::Framebuffer::Create(fbSpec);

	#if MAPTILES
		m_SpriteSheet = Hazel::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

		m_TextureTable = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet,{8,2},{128,128});
		m_TextureStairs = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1,11 }, { 128,128 });

		m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles)/m_MapWidth;

		s_TextureMap['D'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, {6,11}, {128,128});
		s_TextureMap['W'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11,11}, {128,128});
	#endif

	#if PARTICLES
		// Particle props init here
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 2.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };
	#endif
		m_CameraController.SetZoomLevel(5.0f);
	
		} 
	void EditorLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();

	} 

	void EditorLayer::OnUpdate(Hazel::Timestep ts)
	{
		//Timer timer("Sandbox2D::OnUpdate", [&](ProfileResult profileRestult) { m_ProfileResults.push_back(profileRestult); });
		HZ_PROFILE_FUNCTION();
	
	
		// Update
		{
			HZ_PROFILE_SCOPE("CameraController::OnUpdate");
			if(m_ViewportFocused&&m_ViewportHovered)
				m_CameraController.OnUpdate(ts);
		}
	

		// Render
		Hazel::Renderer2D::ResetStats();

		{
			HZ_PROFILE_SCOPE("Renderer Prep");

			m_Framebuffer->Bind();
			Hazel::RenderCommand::SetClearColor({ 0.1, 0.13, 0.13, 1 });
			Hazel::RenderCommand::Clear();
		}

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
			m_Framebuffer->Unbind();
		}


	/* Map Tiles (sprite sheet) rendering */
	#if MAPTILES

		/*2d sub textures map tiles rendering*/ 
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (uint32_t y = 0; y < m_MapHeight; y++)
		{
			for (uint32_t x = 0; x < m_MapWidth; x++)
			{
				char tileType = s_MapTiles[x + y * m_MapWidth];
				Hazel::Ref<Hazel::SubTexture2D> texture;
				if (s_TextureMap.find(tileType) != s_TextureMap.end())
				{
					texture = s_TextureMap[tileType];
				}
				else
				{
					texture = m_TextureStairs;
				}
				Hazel::Renderer2D::DrawQuad({ m_MapWidth / 2.0f - x,y - m_MapHeight / 2.0f ,0.5f }, { 1.0f,1.0f }, texture);
			}
		}
		Hazel::Renderer2D::EndScene();
		
	#endif
	


		//Hazel::Renderer2D::EndScene();
	}
	void EditorLayer::OnImGuiRender()
	{
		HZ_PROFILE_FUNCTION();
	
		
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}
		else
		{
			//Docking wont be disabled
				//ShowDockingDisabledMessage();
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
				ImGui::Separator();
				if (ImGui::MenuItem("Exit"))	Hazel::Application::Get().Close();
				ImGui::EndMenu();
			}


			ImGui::EndMenuBar();
		}
		ImGui::Begin("Settings");

		auto stats = Hazel::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());


		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();


		/* Create the viewport */
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});//设置边框为0
		//为什么这操作保证了GetRegion得到的数值是大于0的？如果没有上面的语句，GetRegion的y值会小于0，这样在resize时创建新的纹理会报错
		ImGui::Begin("Viewport");


		m_ViewportFocused = ImGui::IsWindowFocused();//获取当前这个Viewport（Imgui window）是否被是focused
		m_ViewportHovered = ImGui::IsWindowHovered();//获取当前这个Viewport（Imgui window）是否被是Hovered
		//HZ_CORE_INFO("Focused:{0}",m_ViewportFocused);
		//HZ_CORE_INFO("Hovered:{0}",m_ViewportHovered);
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
				//如果没被focused或没被hovered（一旦鼠标离开viewport区域），就not blockEvents，即允许ImGuiLayer对ImGui::IO的事件（例如鼠标滚轮滑动）进行处理（其实是由ImGUi层没收该事件）

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}

		//HZ_WARN("Viewport Size: {0}, {1}", viewportPanelSize.x, viewportPanelSize.y);
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();//如果上面Resize时重新绑定了纹理附件，那么这一次获取到的纹理附件ID对应的内存是没有正确内容的？？
		ImGui::Image((void*)textureID, viewportPanelSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
		

	}
	void EditorLayer::OnEvent(Hazel::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}