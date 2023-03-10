#include "Precompile.h"
#include "Shader.h"
#include "PlatForm/OpenGL/OpenGLShader.h"

namespace Engine
{
	Ref<Shader> Shader::Create(const std::string& vertexSrcPath, const std::string& fragmentSrcPath, const std::string& geometrySrcPath)
	{
		return CreateRef<OpenGLShader>(vertexSrcPath, fragmentSrcPath, geometrySrcPath);
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource)
	{
		return CreateRef<OpenGLShader>(name, vertexSource, fragmentSource, geometrySource);
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		ENGINE_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		ENGINE_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& vertexSrcPath, const std::string& fragmentSrcPath, const std::string& geometrySrcPath)
	{
		auto shader = Shader::Create(vertexSrcPath, fragmentSrcPath, geometrySrcPath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySource)
	{
		auto shader = Shader::Create(name, vertexSrc, fragmentSrc, geometrySource);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		ENGINE_CORE_ASSERT(Exists(name), "Shader does not exists!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}