#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

#include "RenderCommand.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Model.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform, bool skybox = false, bool withCamera = true);
		static void SubmitInstanced(uint32_t rendererId, GLenum mode, uint32_t count, uint32_t amount, const Ref<Shader>& shader, const glm::mat4& transform);
		static Ref<ShaderLibrary> GetShaderLibrary() { return s_ShaderLibrary; }
		static Ref<ModelLibrary> GetModelLibrary() { return s_ModelLibrary; }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::mat4 ProjectionMatrix;
			glm::mat4 ViewMatrix;
			glm::vec3 CameraPosition;
		};

		static SceneData* s_SceneData;
		static Ref<ShaderLibrary> s_ShaderLibrary;
		static Ref<ModelLibrary> s_ModelLibrary;
	};
}
