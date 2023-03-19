#include "ShadowMap.h"
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
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

	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/ShadowMap.vert", "assets/shaders/ShadowMap.frag");
	m_Shader->Bind();

	m_LightShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/LightCube.vert", "assets/shaders/LightCube.frag");
	m_DepthShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/depth.vert", "assets/shaders/depth.frag");

	m_FrameBuffer = Engine::FrameBuffer::Create();
	m_DepthTexture = Engine::TextureDepthMap::Create(1024, 1024);

	m_FrameBuffer->SetTexture2D(std::dynamic_pointer_cast<Engine::OpenGLTextureDepthMap>(m_DepthTexture)->GetRendererID());
	m_FrameBuffer->Unbind();

	m_ModelTexture->AddMaterialTexture(m_DepthTexture);

	m_PlaneMesh = Engine::Mesh::Create(m_PlaneGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);
	m_BoxMesh = Engine::Mesh::Create(m_BoxGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);
	m_LightMesh = Engine::Mesh::Create(m_LightGeometry, {});
}


void ShadowMap::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 0.0, 0.0, 0.0, 1.0 });

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(m_LightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	m_DepthShader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_DepthShader)->SetMat4("u_ViewProjection", lightSpaceMatrix);
	// render scene from light's point of view

	Engine::RenderCommand::SetViewport(0, 0, m_DepthTexture->GetWidth(), m_DepthTexture->GetHeight());
	m_FrameBuffer->Bind();
	Engine::RenderCommand::DepthClear();
	RenderScene(m_DepthShader, false);
	m_FrameBuffer->Unbind();

	Engine::RenderCommand::SetViewport(0, 0, 1600, 900);
	Engine::RenderCommand::Clear();

	m_LightShader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_LightShader)->SetFloat3("u_LightColor", m_LightColor);
	m_LightGeometry->SetTransform(glm::translate(glm::mat4(1.0f), m_LightPos));
	m_LightMesh->Submit(m_LightShader);

	m_Shader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetMat4("u_LightSpaceMatrix", lightSpaceMatrix);
	RenderScene(m_Shader);

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
	ImGui::SliderFloat3("Light Color", glm::value_ptr(m_LightColor), 0.0f, 1.0f);


	//ImGui::Button("Phong");
	ImGui::End();

}

void ShadowMap::RenderScene(Engine::Ref<Engine::Shader> shader, bool withCamera)
{
	shader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(shader)->SetInt("u_Blinn", m_Blinn);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(shader)->SetInt("u_Gamma", m_Gamma);

	std::dynamic_pointer_cast<Engine::OpenGLShader>(shader)->SetFloat3("u_LightPos", m_LightPos);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(shader)->SetFloat3("u_LightColor", m_LightColor);
	m_PlaneMesh->Submit(m_Shader, withCamera);

	for (uint32_t i = 0; i < 3; i++)
	{
		m_BoxGeometry->SetTransform(m_BoxTransforms[i]);
		m_BoxMesh->Submit(shader, withCamera);
	}

}
