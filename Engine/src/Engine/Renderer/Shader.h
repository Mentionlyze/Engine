#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& vertexSrcPath, const std::string& fragmentSrcPath, const std::string& geometrySrcPath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& vertexSrcPath, const std::string& fragmentSrcPath, const std::string& geometrySrcPath = {});
		Ref<Shader> Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySource);
		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}


