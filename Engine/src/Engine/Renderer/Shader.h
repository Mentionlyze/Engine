#pragma once

namespace Engine
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

		static Shader* Create(const std::string& vertexSource, const std::string& fragmentSource)
		{
			return new Shader(vertexSource, fragmentSource);
		}

	private:
		uint32_t m_RendererID;
	};
}


