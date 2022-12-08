#pragma once

#include "Engine/Renderer/Shader.h"

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Engine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrcPath, const std::string& fragmentSrcPath);
		OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual const std::string& GetName() const override { return m_Name; }

	
		void SetFloat(const std::string& name, float value);
		void SetInt(const std::string& name, uint32_t value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetMat4(const std::string& name, const glm::mat4& value);

		
		void UpdateFloat3(const std::string& name, const glm::vec3& value);
		void UpdateUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		//CompileShader
		std::string ReadFile(const std::string& sourcePath);
		std::unordered_map<GLenum, std::string> Preprocess(const std::string& vertexSource, const std::string& fragmentSource);
		void Compile(const std::unordered_map<GLenum, std::string> shaderSources);
	private:
		uint32_t m_RendererID = 0;
		std::string m_Name;
	};
}

