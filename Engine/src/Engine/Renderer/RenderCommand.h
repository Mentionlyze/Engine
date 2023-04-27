#pragma once

#include "VertexArray.h"
#include "RendererAPI.h"

namespace Engine
{
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DepthClear()
		{
			s_RendererAPI->DepthClear();
		}

		inline static void EnableCullFace()
		{
			s_RendererAPI->EnableCullFace();
		}

		inline static void DisableCullFace()
		{
			s_RendererAPI->DisableCullFace();
		}

		inline static void EnableCubeMapSeamless()
		{
			s_RendererAPI->EnableCubeMapSeamless();
		}


		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawIndexedStrip(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexedStrip(vertexArray);
		}

		inline static void DrawElementInstanced(GLenum mode, uint32_t count, uint32_t amount)
		{
			s_RendererAPI->DrawElementInstanced(mode, count, amount);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}
