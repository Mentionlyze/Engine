#include "BlinnPhong.h"
#include <imgui/imgui.h>

BlinnPhong::BlinnPhong() : Layer("Blinn Phong") , m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 100.f)
{
	m_PlaneGeometry = Engine::Geometry::CreatePlane(glm::mat4(1.0f));
	m_ModelTexture = Engine::ModelTexture::Create();
	m_ModelTexture->AddMaterialTexture("assets/textures/wood.png", "texture_diffuse", true);
	m_Mesh = Engine::Mesh::Create(m_PlaneGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);
	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/BlinnPhong.vert", "assets/shaders/BlinnPhong.frag");
	m_Shader->Bind();
}

void BlinnPhong::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 0.0, 0.0, 0.0, 1.0 });

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());
	
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_LightPos", glm::vec3(0.0));
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3fv("u_LightPositions", 4, m_lightPositions[0]);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3fv("u_LightColors", 4, m_lightColors[0]);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetInt("u_Blinn", m_Blinn);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetInt("u_Gamma", m_Gamma);

	m_Mesh->Submit(m_Shader);

	Engine::Renderer::EndScene();

}

void BlinnPhong::OnEvent(Engine::Event& e)
{
}

void BlinnPhong::OnImGuiRender()
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


	//ImGui::Button("Phong");
	ImGui::End();
}
