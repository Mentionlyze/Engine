#include "ShadowMap.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

ShadowMap::ShadowMap() : Layer("ShadowMap"), 
	m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 1000.0f)
{
	glm::mat4 planeTransform = glm::scale(glm::mat4(1.0f), glm::vec3(2.5f));

	m_PlaneGeometry = Engine::Geometry::CreatePlane(planeTransform);
	m_BoxGeometry = Engine::Geometry::CreateBox(glm::mat4(1.0f));
	m_LightGeometry = Engine::Geometry::CreateBox(glm::translate(glm::mat4(1.0f), m_LightPos));

	m_ModelTexture = Engine::ModelTexture::Create();
	m_ModelTexture->AddMaterialTexture("assets/textures/wood.png", "texture_diffuse", true);

	m_PlaneMesh = Engine::Mesh::Create(m_PlaneGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);
	m_BoxMesh = Engine::Mesh::Create(m_BoxGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);
	m_LightMesh = Engine::Mesh::Create(m_LightGeometry, {});

	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/BlinnPhong.vert", "assets/shaders/ShadowMap.frag");
	m_Shader->Bind();

	m_LightShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/LightCube.vert", "assets/shaders/LightCube.frag");
}

void ShadowMap::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 0.0, 0.0, 0.0, 1.0 });



	Engine::Renderer::BeginScene(m_CameraController.GetCamera());
	m_LightShader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_LightShader)->SetFloat3("u_LightColor", m_LightColor);
	m_LightGeometry->SetTransform(glm::translate(glm::mat4(1.0f), m_LightPos));
	m_LightMesh->Submit(m_LightShader);

	m_Shader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetInt("u_Blinn", m_Blinn);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetInt("u_Gamma", m_Gamma);

	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_LightPos", m_LightPos);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_LightColor", m_LightColor);
	m_PlaneMesh->Submit(m_Shader);


	for (uint32_t i = 0; i < 3; i++)
	{
		m_BoxGeometry->SetTransform(m_BoxTransforms[i]);
		m_BoxMesh->Submit(m_Shader);
	}


	Engine::Renderer::EndScene();
}

void ShadowMap::OnEvent(Engine::Event& e)
{
}

void ShadowMap::OnImGuiRender()
{
	ImGui::Begin("Advanced Light");
	if (ImGui::Button("Blinn Phong"))
	{
		m_Blinn = true;
	}
	if (ImGui::Button("Phong"))
	{
		m_Blinn = false;
	}

	if (ImGui::Button("Gamma"))
	{
		m_Gamma = true;
	}
	if (ImGui::Button("Clear Gamma"))
	{
		m_Gamma = false;
	}

	ImGui::SliderFloat3("Light Pos", glm::value_ptr(m_LightPos), -10.0f, 10.0f);


	//ImGui::Button("Phong");
	ImGui::End();

}
