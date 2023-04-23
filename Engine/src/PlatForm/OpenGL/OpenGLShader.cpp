#include "Precompile.h"
#include "OpenGLShader.h"
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		ENGINE_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	static std::string ShaderTypeToString(const GLenum type)
	{
		if (type == GL_VERTEX_SHADER)
			return "vertexShader";
		if (type == GL_FRAGMENT_SHADER)
			return "fragmentShader";

		ENGINE_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}


	OpenGLShader::OpenGLShader(const std::string& vertexSrcPath, const std::string& fragmentSrcPath, const std::string& geometrySrcPath)
	{
		std::string vertextShaderSource  = ReadFile(vertexSrcPath);
		std::string fragmentShaderSource = ReadFile(fragmentSrcPath);
		std::string geometryShaderSource = ReadFile(geometrySrcPath);

		std::unordered_map<GLenum, std::string> shaderSources = Preprocess(vertextShaderSource, fragmentShaderSource, geometryShaderSource);
		Compile(shaderSources);

		auto lastSlash = vertexSrcPath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = vertexSrcPath.rfind('.');
		auto count = lastDot == std::string::npos ? vertexSrcPath.size() - lastSlash : lastDot - lastSlash;
		m_Name = vertexSrcPath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource): m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shaderSources = Preprocess(vertexSource, fragmentSource, geometrySource);
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader() 
	{
		glDeleteProgram(m_RendererID);
	}


	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		if (filePath.empty())
			return {};

		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			ENGINE_CORE_ERROR("Could not open file: {0}", filePath);
		}
		return result;
	}


	std::unordered_map<GLenum, std::string> OpenGLShader::Preprocess(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		shaderSources[GL_VERTEX_SHADER] = vertexSource;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSource;
		if (!geometrySource.empty())
			shaderSources[GL_GEOMETRY_SHADER] = geometrySource;

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> shaderSources)
	{
		GLuint program= glCreateProgram();
		ENGINE_CORE_ASSERT(shaderSources.size() == 2 || shaderSources.size() == 3, "We only suppot 2 or 3 shaders for now");
		std::array<GLenum, 3> glShaderIds;
		int glShaderIdIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			// Read our shaders into the appropriate buffers
			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(type);
			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				ENGINE_CORE_ERROR("{0}", infoLog.data());
				ENGINE_CORE_ASSERT(false, "shader compilation failed!");

				// In this simple program, we'll just leave
				return;
			}

			glAttachShader(program, shader);
			glShaderIds[glShaderIdIndex++] = shader;
		}
		
		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShaderIds)
				glDeleteShader(id);

			// Use the infoLog as you see fit.
			ENGINE_CORE_ERROR("{0}", infoLog.data());
			ENGINE_CORE_ASSERT(false, "Program link failed!");

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIds)
			glDeleteShader(id);

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = program;
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::SetInt(const std::string& name, uint32_t value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UpdateFloat3(name, value);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		UpdateUniformMat3(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UpdateUniformMat4(name, value);
	}

	void OpenGLShader::SetFloat3fv(const std::string& name, uint32_t count, const glm::vec3& value)
	{
		UpdateFloat3fv(name, count, value);
	}


	void OpenGLShader::UpdateFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UpdateFloat3fv(const std::string& name, uint32_t count, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3fv(location, count, glm::value_ptr(value));
	}

	void OpenGLShader::UpdateUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UpdateUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}
}
