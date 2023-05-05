#pragma once

#include "Hazel.h"

namespace Hazel {

	class EditorLayer :public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;



	private:
		Hazel::OrthographicCameraController m_CameraController;


		Hazel::Ref<Texture2D> m_CheckerboardTexture;
		Hazel::Ref<Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportSize = {0.0f, 0.0f};

		glm::vec4 m_SquareColor = { 0.2f,0.3f,0.8f, 1.0f };
#if MAPTILES
		Hazel::Ref<Hazel::Texture2D> m_SpriteSheet;
		Hazel::Ref<Hazel::SubTexture2D> m_TextureStairs;
		Hazel::Ref<Hazel::SubTexture2D> m_TextureTable;

		uint32_t m_MapWidth, m_MapHeight;
		std::unordered_map<char, Hazel::Ref<Hazel::SubTexture2D>> s_TextureMap;
#endif


	};
}