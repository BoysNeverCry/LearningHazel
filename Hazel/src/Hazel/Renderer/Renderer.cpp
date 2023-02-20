#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {
	
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{//保证我们的shader使用了那些正确的uniforms，这些uniforms确定了：environmentmap、cubemap、camera细节等等
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
		//发送到Render command queue,由RenderCommand完成计算和处理
		RenderCommand::DrawIndexed(vertexArray);
	}

}