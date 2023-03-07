#include "hzpch.h"
#include "Renderer.h"

#include "platform/OpenGL/OpenGLShader.h"

namespace Hazel {
	
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{//��֤���ǵ�shaderʹ������Щ��ȷ��uniforms����Щuniformsȷ���ˣ�environmentmap��cubemap��cameraϸ�ڵȵ�
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);//��ʵ��VP������ϴ�����Ҫ��ÿ��drawcallǰ���ã�������ÿ�������ж�ÿ���������һ�ξ���
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		//���͵�Render command queue,��RenderCommand��ɼ���ʹ���
		RenderCommand::DrawIndexed(vertexArray);
	}

}