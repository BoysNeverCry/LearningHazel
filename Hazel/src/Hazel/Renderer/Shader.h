#pragma once

#include<string>
#include<unordered_map>

#include <glm/glm.hpp>

namespace Hazel {

	class Shader
	{
	public:

		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual void SetInt(const std::string& name, const int value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		//virtual void UploadUniformBuffer
		//如何实现这个上传uniform的功能？会有一个shader pointer，我们把这个shader cast类型转换成openglshader，再直接调用openglshader中的上传函数？？？难道是向下动态转换？
		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

	};

	class ShaderLibrary//理应由renderer类来管理
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& s);
		void Add(const Ref<Shader>& s);
		Ref<Shader> Load(const std::string& filepath); // assets/Texture.glsl
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
} 