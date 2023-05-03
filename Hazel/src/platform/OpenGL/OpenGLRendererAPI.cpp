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
		glViewport(x, y, width, height);//x,y�����½ǵ��������꣬width,height�����Ͻǵ���������
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
		glBindTexture(GL_TEXTURE_2D, 0);//��id��0��GL_TEXTURE_2D������󵽵�ǰ���������Ԫ����֮ǰactivate������Ԫ��
										//����id��0,����Ӧ�κ�����������������ǰѵ�ǰ���������Ԫ�ϵ����������
	}	

}