#include "Precompile.h"
#include "Renderer.h"
#include "PlatForm/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Engine
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;
	Ref<ShaderLibrary> Renderer::s_ShaderLibrary = CreateRef<ShaderLibrary>();
	Ref<ModelLibrary> Renderer::s_ModelLibrary = CreateRef<ModelLibrary>();

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_SceneData->ProjectionMatrix = camera.GetProjectionMatrix();
		s_SceneData->ViewMatrix = camera.GetViewMatrix();
		s_SceneData->CameraPosition = camera.GetPosition();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform, bool skybox)
	{
		shader->Bind();

		if (skybox) 
		{
			glm::mat4 projectiMatrix = s_SceneData->ProjectionMatrix;
			glm::mat4 viewMatrix = glm::mat4(glm::mat3(s_SceneData->ViewMatrix));

			std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_ViewProjection", projectiMatrix * viewMatrix);
		}
		else 
		{
			std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		}
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_Projection", s_SceneData->ProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_View", s_SceneData->ViewMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_Transform", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateFloat3("u_ViewPosition", s_SceneData->CameraPosition);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::SubmitInstanced(uint32_t rendererId, GLenum mode, uint32_t count, uint32_t amount, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		shader->Bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_Projection", s_SceneData->ProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_View", s_SceneData->ViewMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_Transform", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateFloat3("u_ViewPosition", s_SceneData->CameraPosition);

		glBindVertexArray(rendererId);
		RenderCommand::DrawElementInstanced(mode, count, amount);
		glBindVertexArray(0);
	}
}