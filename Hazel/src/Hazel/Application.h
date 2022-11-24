#pragma once

#include "Core.h"

#include "Window.h"
#include "Events/Event.h"

#include "Hazel\Events\ApplicationEvent.h"

namespace Hazel {

	class  HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		std::unique_ptr<Window> m_Window;

		bool m_Running = true;

		bool OnWindowClose(WindowCloseEvent& e);
	};
	 
	//To be defined in CLIENT
	Application* CreateApplication();


}


