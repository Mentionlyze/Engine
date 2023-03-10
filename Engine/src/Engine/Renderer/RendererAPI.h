#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"
#include <glad/glad.h>

namespace Engine
{

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

		static API GetAPI() { return s_API; }

	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		virtual void DrawElementInstanced(GLenum mode, uint32_t count, uint32_t amount) = 0;

	private:
		static API s_API;
	};
}


