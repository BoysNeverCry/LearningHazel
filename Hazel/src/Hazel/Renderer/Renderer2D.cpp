#include "hzpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Hazel {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		//Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();
		//HZ_CORE_ERROR("test________");
		s_Data = new Renderer2DStorage();
		
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({//���ö������ݵ�BufferLayout������һ���Զ������Ͷ���
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0,1,2,2,3,0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		
		//s_Data->TextureShader = Shader::Create("assets/shaders/FlatColor.glsl");
		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture",0);//��ǰָ���ã����ǵ������ᱻ�󶨵�������ۺţ����ᱻ���ݸ�Ƭ����ɫ��
	}

	void Renderer2D::Shutdown()
	{
		HZ_PROFILE_FUNCTION();

		//�����ֶ������ڴ�
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HZ_PROFILE_FUNCTION();

		//s_Data->FlatColorShader->Bind();
		//s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		
	}

	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{

		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		//s_Data->FlatColorShader->Bind();//��һ������Ҫ���ǰ�ȫ����������°�һ��
		s_Data->TextureShader->SetFloat4("u_Color", color);
		// Bind white texture here
		s_Data->WhiteTexture->Bind();

		//���㱻���ƶ����transform,����˳���ǣ�translation * rotation * scale
		glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) * /* rotation */	glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}



	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		HZ_PROFILE_FUNCTION();

		//s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();//�����ָ��������Ĭ�ϻ��0���������

		//���㱻���ƶ����transform,����˳���ǣ�translation * rotation * scale
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * /* rotation */ glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}