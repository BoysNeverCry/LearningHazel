#include <Hazel.h>
//---Entry Point------------------------
#include "Hazel/Core/EntryPoint.h"
//--------------------------------------
#include "platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"


#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"


class ExampleLayer : public Hazel::Layer 
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f,true)/*,m_SquarePosition(0.0f)*/
	{
		// Vertex Array
		// Vertex Buffer
		// Index Buffer

		m_VertexArray = Hazel::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.9f, 0.8f, 0.2f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
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
		Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));//创建索引缓冲区对象，并把索引数据传入
		m_VertexArray->SetIndexBuffer(indexBuffer);
		///////////////////////////SQUARE//////////////////////
		m_SquareVA = (Hazel::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({//配置顶点数据的BufferLayout，这是一个自定义类型对象
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0,1,2,2,3,0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertecSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position,1.0);
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

		m_Shader = Hazel::Shader::Create("VertexPosColor", vertecSrc, fragmentSrc);

		

		std::string flatColorShaderVertecSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position,1.0);
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;

			uniform vec3 u_Color;
			
			void main()
			{
				color = vec4(u_Color,1.0);

			}
		)";

		m_FlatColorShader = Hazel::Shader::Create("FlatColor", flatColorShaderVertecSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		//m_TextureShader = Hazel::Shader::Create("assets/shaders/Texture.glsl");

		m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");

		m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/grass.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		Hazel::RenderCommand::SetClearColor({ 0.1, 0.13, 0.13, 1 });
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

		
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));//缩放变换矩阵

		//glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		//glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f,y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);

			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");
		
		m_Texture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_ChernoLogoTexture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVA,  glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		//Triangle
		//Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		/*ImGui::Begin("test");
		ImGui::Text("Hello Owen");
		ImGui::ColorEdit4("",new float[4]);
		ImGui::End();*/
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& e) override
	{
		//Event Dispatching
		/*Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnkeyPressedEvent));*/
		//Dispatch的工作：如果event的事件类型与形参Hazel::KeyPressedEvent相同，
				//那么执行函数对象OnkeyPressedEvent对event进行处理，否则不进行处理
		m_CameraController.OnEvent(e);

		
	
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
		Hazel::ShaderLibrary m_ShaderLibrary;

		Hazel::Ref<Hazel::Shader> m_Shader;
		Hazel::Ref<Hazel::VertexArray> m_VertexArray;
		
		Hazel::Ref<Hazel::Shader> m_FlatColorShader;
		Hazel::Ref<Hazel::VertexArray> m_SquareVA;

		Hazel::Ref<Hazel::Texture2D> m_Texture; 
		Hazel::Ref<Hazel::Texture2D> m_ChernoLogoTexture;

		Hazel::OrthographicCameraController m_CameraController;



		glm::vec3 m_SquareColor = {0.2f,0.3f,0.7f};
		/*glm::vec3 m_SquarePosition;
		float m_SquareMoveSpeed = 1.0f;*/
};



class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
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

