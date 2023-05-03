#include "hzpch.h"
#include "OpenGLRendererAPI.h"

#include<glad/glad.h>

namespace Hazel {
	void OpenGLRendererAPI::Init()
	{
		HZ_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);//x,y是左下角的像素坐标，width,height是右上角的像素坐标
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Hazel::Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{

		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		//HZ_CORE_INFO("IndexCount: {0}", count);
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

		/*uint32_t count = indexCount>0 ? indexCount: VertexArray->GetIndexBuffer()->GetCount();
		
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);*/
		glBindTexture(GL_TEXTURE_2D, 0);//绑定id是0的GL_TEXTURE_2D纹理对象到当前激活的纹理单元（即之前activate的纹理单元）
										//这里id是0,不对应任何纹理对象，所以作用是把当前激活的纹理单元上的纹理对象解绑
	}	

}