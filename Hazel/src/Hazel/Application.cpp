#include "hzpch.h"
#include "Application.h"

#include "Hazel\Log.h"


#include "Hazel/Renderer/Renderer.h"

#include "Input.h"



namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x,this,  std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		:m_Camera(-1.6f,1.6f,-0.9f,0.9f)
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// Vertex Array
		// Vertex Buffer
		// Index Buffer

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.9f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices,sizeof(vertices)));//把顶点数据加入到VertexBuffer中
		BufferLayout layout = {//配置顶点数据的BufferLayout，这是一个自定义类型对象
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		};
		vertexBuffer->SetLayout(layout);//我们的自定义VertexBuffer对象除了拥有顶点数据，也保管有这个布局layout
		m_VertexArray->AddVertexBuffer(vertexBuffer);//把我们的VertexBuffer添加到VertexArray中，即绑定起来
																	/*	1.绑定VertexArray对象，即后续操作都与当前VertexArray关联
																		2.绑定VertexBuffer对象
																		3.遍历layout的element，具体指定属性布局
																	*/
		unsigned int indices[3] = {	0,1,2};
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices,sizeof(indices)/sizeof(uint32_t)));//创建索引缓冲区对象，并把索引数据传入
		m_VertexArray->SetIndexBuffer(indexBuffer);
		///////////////////////////SQUARE//////////////////////
		m_SquareVA.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset( VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({//配置顶点数据的BufferLayout，这是一个自定义类型对象
			{ ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0,1,2,2,3,0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertecSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position,1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position+(0.5,0.5,0.5),1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(new Shader(vertecSrc,fragmentSrc));

		std::string blueShaderVertecSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position,1.0);
			}
		)";
		
		std::string blueShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			
			void main()
			{
				color = vec4(0.2,0.3,0.8,1.0);

			}
		)";

		m_BlueShader.reset(new Shader(blueShaderVertecSrc, blueShaderFragmentSrc));
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		HZ_CORE_TRACE("{0}",e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		float R = 0;
		while(m_Running)
		{
			R += 0.2f;
			/*glClearColor(0.1, 0.13, 0.13, 1);
			glClear(GL_COLOR_BUFFER_BIT);*/
			RenderCommand::SetClearColor({ 0.1, 0.13, 0.13, 1 });
			RenderCommand::Clear();


			//尝试改变相机位置
			m_Camera.SetPosition({ 0.5f,0.5f,0.0f });
			m_Camera.SetRotation(45.0f);

			Renderer::BeginScene(m_Camera);	

			Renderer::Submit(m_BlueShader,m_SquareVA);

			Renderer::Submit(m_Shader,m_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			/*auto [x, y] = Input::GetMousePosition();
			HZ_CORE_TRACE("{0},{1}", x, y);*/

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}