#include "hzpch.h"
#include "Application.h"

#include "Hazel\Events\ApplicationEvent.h"
#include "Hazel\Log.h"

#include <GLFW\glfw3.h>


namespace Hazel {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while(m_Running)
		{
			glClearColor(0.5, 0.3, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

}