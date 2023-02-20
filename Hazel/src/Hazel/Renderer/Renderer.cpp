#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {
	
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{//��֤���ǵ�shaderʹ������Щ��ȷ��uniforms����Щuniformsȷ���ˣ�environmentmap��cubemap��cameraϸ�ڵȵ�
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader,const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

		vertexArray->Bind();
		//���͵�Render command queue,��RenderCommand��ɼ���ʹ���
		RenderCommand::DrawIndexed(vertexArray);
	}

}