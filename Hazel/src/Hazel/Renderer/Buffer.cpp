#include "hzpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "platform/OpenGL/OpenGLBuffer.h"


namespace Hazel {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");	return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(size);
		}

		HZ_CORE_ASSERT(false, "Unknown rendererAPI");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");	return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		HZ_CORE_ASSERT(false, "Unknown rendererAPI");
		return nullptr;
	}

	


	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");	return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		HZ_CORE_ASSERT(false, "Unknown rendererAPI");
		return nullptr;
	}

}