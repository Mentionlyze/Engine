#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

#include "RenderCommand.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Camera.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void Init();
		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform);
		static Ref<ShaderLibrary> GetShaderLibrary() { return s_ShaderLibrary; }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
		static Ref<ShaderLibrary> s_ShaderLibrary;
	};
}
