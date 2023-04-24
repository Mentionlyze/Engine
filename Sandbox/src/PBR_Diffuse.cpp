#include "PBR_Diffuse.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

PBR_Diffuse::PBR_Diffuse() : Layer("PBR Diffuse"), m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 1000.0f)
{
	m_SphereGeometry = Engine::Geometry::CreateSphere(glm::mat4(1.0f));
	m_SphereMesh = Engine::Mesh::Create(m_SphereGeometry, {});
	
	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/PBR.vert", "assets/shaders/PBR.frag");
	m_Shader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("ao", 1.0f);

}

void PBR_Diffuse::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::SetClearColor({ 0, 0, 0, 1 });
	Engine::RenderCommand::Clear();

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());


	for (int row = 0; row < nrRows; ++row)
	{
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("metallic", (float)row / (float)nrRows);
		for (int col = 0; col < nrColumns; ++col)
		{
			
			std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("roughness",
					glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f)
				);

			glm::mat4 transfom = glm::translate(glm::mat4(1.0f), glm::vec3(
				(col - (nrColumns/ 2)) * spacing,
				(row - (nrRows/ 2)) * spacing,
				0.0
			));

			m_SphereGeometry->SetTransform(transfom);

			std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetMat3("normalMatrix", 
					glm::transpose(glm::inverse(glm::mat3(transfom)))
				);

			m_SphereMesh->SubmitStrip(m_Shader);
		}
	}

	for (int i = 0; i < sizeof(m_LightPositions) / sizeof(m_LightPositions[0]); ++i)
	{
		glm::vec3 newPos = m_LightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = m_LightPositions[i];
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("lightPositions[" + std::to_string(i) + "]", newPos);
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("lightColors[" + std::to_string(i) + "]", m_LightColors[0]);

		glm::mat4 transfom = glm::scale(glm::translate(glm::mat4(1.0f), newPos), glm::vec3(0.5f));

		m_SphereGeometry->SetTransform(transfom);

		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetMat3("normalMatrix",
			glm::transpose(glm::inverse(glm::mat3(transfom)))
		);

		m_SphereMesh->SubmitStrip(m_Shader);
	}

	Engine::Renderer::EndScene();

}

void PBR_Diffuse::OnEvent(Engine::Event& e)
{
}

void PBR_Diffuse::OnImGuiRender()
{
}
