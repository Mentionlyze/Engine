#pragma once

#include "Engine/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Engine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& path);
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	
		void SetInt(const std::string& name, uint32_t value);
		void setFloat3(const std::string& name, const glm::vec3& value);
		void SetMat4(const std::string& name, const glm::mat4& value);

		
		void UpdateFloat3(const std::string& name, const glm::vec3& value);
		void UpdateUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID = 0;
	};
}

