#pragma once

#include "Core.h"

#include "Window.h"
#include "Hazel/Core/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel\Events\ApplicationEvent.h"

#include "hazel/Core/Timestep.h"

#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel {

	class  HAZEL_API Application
	{
	public:
		Application(const std::string& name = "Hazel app");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Close();

		inline static Application& Get() { return *s_Instance; }
			
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack  m_LayerStack;

		float m_LastFrameTime = 0.0f;
		
	private:
		static Application* s_Instance;
	};
	 
	//To be defined in CLIENT
	Application* CreateApplication();


}


