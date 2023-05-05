#include <Hazel.h>
//---Entry Point------------------------
#include "Hazel/Core/EntryPoint.h"
//--------------------------------------

#include "EditorLayer.h"

namespace Hazel{

	class Hazelnut : public Hazel::Application
	{
	public:
		Hazelnut()
			: Application("Hazelnut")
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
			//PushLayer(new Hazel::ImGuiLayer());
		}

		~Hazelnut()
		{

		}


	};

	Hazel::Application* Hazel::CreateApplication()
	{
		return new Hazelnut();
	}
}

