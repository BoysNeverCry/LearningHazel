#pragma once


namespace Hazel {

	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		//FrambufferFormat Format =
		uint32_t Samples = 1;

		// ���SwapChainTarget����˼���ǣ���ǰ������Ļ��Ⱦ���߲���
		// Render Pass����һ������� Frame buffer
		// ��OpenGL�����ϣ��ֱ����Ⱦ����Ļ����ô���ǲ�ȥ�����κ�framebuffer����
		bool SwapChainTarget = false;// �ȼ���glBindFramebuffer(0)��Ҳ����˵��ǰ��Ⱦ����Ĭ��֡���壨0�ţ�
		

	};

	class Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width,uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

	};
}