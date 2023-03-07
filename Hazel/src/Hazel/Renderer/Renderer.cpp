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
	{//保证我们的shader使用了那些正确的uniforms，这些uniforms确定了：environmentmap、cubemap、camera细节等等
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);//事实上VP矩阵的上传不需要在每次drawcall前调用，而是在每个场景中对每个物体调用一次就行
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		//发送到Render command queue,由RenderCommand完成计算和处理
		RenderCommand::DrawIndexed(vertexArray);
	}

}