#include "Precompile.h"
#include "Renderer.h"
#include "PlatForm/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;
	Ref<ShaderLibrary> Renderer::s_ShaderLibrary = CreateRef<ShaderLibrary>();

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
		s_SceneData->ViewMatrix = camera.GetViewMatrix();
		s_SceneData->CameraPosition = camera.GetPosition();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		shader->Bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_View", s_SceneData->ViewMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_Transform", transform);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateFloat3("u_ViewPosition", s_SceneData->CameraPosition);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}