#include "hzpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Hazel {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
	: m_Specification(spec)
	{
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{

	}

	void OpenGLFramebuffer::Invalidate()
	{
		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);//�����������֡����󶨵���Ⱦ������
		
		//����ɫ���嵽֡����
		glCreateTextures(GL_TEXTURE_2D,1,&m_ColorAttachment);//������ɫ��������
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, nullptr);//Ϊ��������ɫ��������ָ����ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//��������Сʱ�������Թ���
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//�������Ŵ�ʱ�������Թ���

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0); 
		//��Texture2D�����������ӵ�֡�����COLOR_ATTACHMENT0�ϡ��ò����������������͵ĸ���������ȵ���
		
		//����Ȼ��嵽֡����
		glCreateTextures(GL_TEXTURE_2D,1,&m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);//Ϊ�����������������ָ����ʽ
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
		//��Texture2D�����������ӵ�֡�����GL_DEPTH_STENCIL_ATTACHMENT��

		HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");//���֡��������������

		glBindFramebuffer(GL_FRAMEBUFFER, 0);//��Ĭ��0��֡���壨��Ļ���壩�󶨵���Ⱦ������
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);//�����������֡����󶨵���Ⱦ������
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}