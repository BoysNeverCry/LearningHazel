#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {
	
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");//the name is only for debug
		virtual ~Layer();

		virtual void OnAttach() {} //put the layer on stack to be part of our application
		virtual void OnDetach() {}
		virtual void OnUpdate() {} //Gets called by the application by upadate which should be every frame
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {} // When events are sent here

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};


}


