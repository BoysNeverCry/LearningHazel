#include <Hazel.h>
#include <stdio.h>

#include "imgui/imgui.h"
#include <string>

class ExampleLayer : public Hazel::Layer 
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		// Vertex Array
		// Vertex Buffer
		// Index Buffer

		m_VertexArray.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.9f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));//把顶点数据加入到VertexBuffer中
		Hazel::BufferLayout layout = {//配置顶点数据的BufferLayout，这是一个自定义类型对象
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" },
		};
		vertexBuffer->SetLayout(layout);//我们的自定义VertexBuffer对象除了拥有顶点数据，也保管有这个布局layout
		m_VertexArray->AddVertexBuffer(vertexBuffer);//把我们的VertexBuffer添加到VertexArray中，即绑定起来
		/*	1.绑定VertexArray对象，即后续操作都与当前VertexArray关联
			2.绑定VertexBuffer对象
			3.遍历layout的element，具体指定属性布局
		*/
		unsigned int indices[3] = { 0,1,2 };
		std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));//创建索引缓冲区对象，并把索引数据传入
		m_VertexArray->SetIndexBuffer(indexBuffer);
		///////////////////////////SQUARE//////////////////////
		m_SquareVA.reset(Hazel::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({//配置顶点数据的BufferLayout，这是一个自定义类型对象
			{ Hazel::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0,1,2,2,3,0 };
		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		m_Shader.reset(new Hazel::Shader(vertecSrc, fragmentSrc));

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

		m_BlueShader.reset(new Hazel::Shader(blueShaderVertecSrc, blueShaderFragmentSrc));
	}

	void OnUpdate() override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation -= m_CameraRotationSpeed;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation += m_CameraRotationSpeed;

		Hazel::RenderCommand::SetClearColor({ 0.1, 0.13, 0.13, 1 });
		Hazel::RenderCommand::Clear();


		//尝试改变相机位置
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_Camera);

		Hazel::Renderer::Submit(m_BlueShader, m_SquareVA);

		Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		/*ImGui::Begin("test");
		ImGui::Text("Hello Owen");
		ImGui::ColorEdit4("",new float[4]);
		ImGui::End();*/
	}

	void OnEvent(Hazel::Event& event) override
	{
		//Event Dispatching
		/*Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnkeyPressedEvent));*/
		//Dispatch的工作：如果event的事件类型与形参Hazel::KeyPressedEvent相同，
				//那么执行函数对象OnkeyPressedEvent对event进行处理，否则不进行处理
	
	}

	//bool OnkeyPressedEvent(Hazel::KeyPressedEvent& event)
	//{
	//	if (event.GetKeyCode() == HZ_KEY_LEFT)
	//		m_CameraPosition.x -= m_CameraSpeed;

	//	if (event.GetKeyCode() == HZ_KEY_RIGHT)
	//		m_CameraPosition.x += m_CameraSpeed;

	//	if (event.GetKeyCode() == HZ_KEY_DOWN)
	//		m_CameraPosition.y -= m_CameraSpeed;

	//	if (event.GetKeyCode() == HZ_KEY_UP)
	//		m_CameraPosition.y += m_CameraSpeed;

	//	return false;
	//}

	private:
		std::shared_ptr<Hazel::Shader> m_Shader;
		std::shared_ptr<Hazel::VertexArray> m_VertexArray;

		std::shared_ptr<Hazel::VertexArray> m_SquareVA;
		std::shared_ptr<Hazel::Shader> m_BlueShader;

		Hazel::OrthographicCamera m_Camera;
		glm::vec3 m_CameraPosition;

		float m_CameraMoveSpeed = 0.05f; 
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 2.f;
};



class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		//PushLayer(new Hazel::ImGuiLayer());
	}

	~Sandbox()
	{

	}


};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}

