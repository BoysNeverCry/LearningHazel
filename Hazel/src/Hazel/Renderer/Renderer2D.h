#pragma once

#include "OrthographicCamera.h"

#include "Texture.h"

namespace Hazel {
	
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();//如果是batch renderer，可能会进行把数据发送到GPU的操作；像目前这样简单的渲染器不会有任何操作

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);//
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);//坐标的第三维度可用于排序
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
	};
}