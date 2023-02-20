#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Hazel {
	
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		//const GLubyte* str = glGetString(GL_RENDERER);
		//std::cout << glGetString(GL_RENDERER) << std::endl;
		//HZ_CORE_INFO("Vendor: {0}",str);
		/*HZ_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		HZ_CORE_INFO("Version: {0}", glGetString(GL_VERSION));*/
	}

	void OpenGLContext::SwapBuffers()
	{

		

		glfwSwapBuffers(m_WindowHandle);
	}

}
