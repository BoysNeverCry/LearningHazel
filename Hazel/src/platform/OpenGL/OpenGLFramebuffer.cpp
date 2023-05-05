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
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{

		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);//把所属对象的帧缓冲绑定到渲染管线上
		
		//绑定颜色缓冲到帧缓冲
		glCreateTextures(GL_TEXTURE_2D,1,&m_ColorAttachment);//创建颜色纹理附件
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, nullptr);//为创建的颜色纹理附件指定格式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//纹理被缩小时采用线性过滤
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//纹理被放大时采用线性过滤

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0); 
		//把Texture2D纹理附件附加到帧缓冲的COLOR_ATTACHMENT0上。该参数可以是其他类型的附件例如深度等上
		
		//绑定深度缓冲到帧缓冲
		glCreateTextures(GL_TEXTURE_2D,1,&m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);//为创建的深度纹理附件指定格式
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
		//把Texture2D纹理附件附加到帧缓冲的GL_DEPTH_STENCIL_ATTACHMENT上

		HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");//检查帧缓冲对象的完整性

		glBindFramebuffer(GL_FRAMEBUFFER, 0);//把默认0号帧缓冲（屏幕缓冲）绑定到渲染管线上
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);//把所属对象的帧缓冲绑定到渲染管线上

		glViewport(0,0,m_Specification.Width,m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

}