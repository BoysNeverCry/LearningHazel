#include <Hazel.h>
//---Entry Point------------------------
#include "Hazel/Core/EntryPoint.h"
//--------------------------------------

#include "EditorLayer.h"

namespace Hazel{

	class HazelEditor : public Hazel::Application
	{
	public:
		HazelEditor()
			: Application("Hazel Editor")
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
			//PushLayer(new Hazel::ImGuiLayer());
		}

		~HazelEditor()
		{

		}


	};

	Hazel::Application* Hazel::CreateApplication()
	{
		return new HazelEditor();
	}
}

