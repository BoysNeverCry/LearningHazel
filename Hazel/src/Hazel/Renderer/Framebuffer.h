#pragma once


namespace Hazel {

	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		//FrambufferFormat Format =
		uint32_t Samples = 1;

		// 这个SwapChainTarget的意思就是：当前是往屏幕渲染或者不是
		// Render Pass就是一个抽象的 Frame buffer
		// 在OpenGL中如果希望直接渲染到屏幕，那么就是不去控制任何framebuffer对象
		bool SwapChainTarget = false;// 等价于glBindFramebuffer(0)，也就是说当前渲染流向默认帧缓冲（0号）
		

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