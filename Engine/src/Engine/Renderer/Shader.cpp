#include "Precompile.h"
#include "Shader.h"
#include "PlatForm/OpenGL/OpenGLShader.h"

namespace Engine
{
	Ref<Shader> Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		return CreateRef<OpenGLShader>(vertexSource, fragmentSource);
	}
}