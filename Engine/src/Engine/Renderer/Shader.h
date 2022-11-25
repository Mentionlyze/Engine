#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetMat4(const std::string& name, const glm::mat4& value);

		void UpdateUniformMat4(const std::string& name, const glm::mat4& matrix);

		static Shader* Create(const std::string& vertexSource, const std::string& fragmentSource)
		{
			return new Shader(vertexSource, fragmentSource);
		}

	private:
		uint32_t m_RendererID;
	};
}


