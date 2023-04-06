#include "PointLightShadow.h"
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

PointLightShadow::PointLightShadow() : Layer("ShadowMap"),
	m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 1000.0f),
	m_ShadowProjection(glm::perspective(90.0f, 1.0f, 0.1f, 25.0f))
{
	Engine::RenderCommand::EnableCullFace();
	Engine::RenderCommand::DepthClear();


	m_CameraController.SetPosition({ 0.0f, 0.0f, 1.0f });

	m_PlaneGeometry = Engine::Geometry::CreatePlane(glm::mat4(1.0f));
	m_BoxGeometry = Engine::Geometry::CreateBox(glm::mat4(1.0f));
	m_LightGeometry = Engine::Geometry::CreateBox(glm::translate(glm::mat4(1.0f), m_LightPos));

	m_ModelTexture = Engine::ModelTexture::Create();
	m_ModelTexture->AddMaterialTexture("assets/textures/wood.png", "texture_diffuse");

	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/PointShadow.vert", "assets/shaders/PointShadow.frag");
	m_Shader->Bind();

	m_DepthShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/PointShadowDepth.vert", "assets/shaders/PointShadowDepth.frag", "assets/shaders/PointShadowDepth.geom");
	m_LightShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/LightCube.vert", "assets/shaders/LightCube.frag");

	m_FrameBuffer = Engine::FrameBuffer::Create();
	m_DepthTexture = Engine::TextureDepthCubeMap::Create(2048, 2048);
	m_DepthTexture->Bind(1);

	m_FrameBuffer->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLFrameBuffer>(m_FrameBuffer)->SetDepthCubeMapTexture(std::dynamic_pointer_cast<Engine::OpenGLTextureDepthCubMap>(m_DepthTexture)->GetRendererID());
	m_FrameBuffer->Unbind();

	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetInt("texture_diffuse", 0);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetInt("depthMap", 1);

	m_ModelTexture->AddMaterialTexture(m_DepthTexture);

	m_PlaneMesh = Engine::Mesh::Create(m_PlaneGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);
	m_BoxMesh = Engine::Mesh::Create(m_BoxGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);
	m_LightMesh = Engine::Mesh::Create(m_LightGeometry, {});
}


void PointLightShadow::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 0.0, 0.0, 0.0, 1.0 });

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	//m_LightPos.z = static_cast<float>(sin(glfwGetTime() * 0.5) * 3.0);

	float near_plane = 0.1f;
	float far_plane = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)2048/ (float)2048, near_plane, far_plane);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));


	Engine::RenderCommand::SetViewport(0, 0, m_DepthTexture->GetWidth(), m_DepthTexture->GetHeight());
	m_FrameBuffer->Bind();
	Engine::RenderCommand::DepthClear();

	m_DepthShader->Bind();
	for (uint32_t i = 0; i < 6; ++i)
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_DepthShader)->SetMat4("u_ShadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);

	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_DepthShader)->SetFloat("u_FarPlane", far_plane);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_DepthShader)->SetFloat3("m_LightPos", m_LightPos);

	RenderScene(m_DepthShader);
	m_FrameBuffer->Unbind();

	Engine::RenderCommand::SetViewport(0, 0, 1600, 900);
	Engine::RenderCommand::Clear();

	m_Shader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("u_FarPlane", far_plane);

	RenderScene(m_Shader);

	m_LightShader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_LightShader)->SetFloat3("u_LightColor", m_LightColor);
	glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), m_LightPos), glm::vec3(0.1f));
	m_LightGeometry->SetTransform(transform);
	m_LightMesh->Submit(m_LightShader);

	Engine::Renderer::EndScene();
}

void PointLightShadow::OnEvent(Engine::Event& e)
{

}

void PointLightShadow::OnImGuiRender()
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

	ImGui::SliderFloat3("Light Pos", glm::value_ptr(m_LightPos), -20.0f,20.0f);
	ImGui::SliderFloat3("Light Color", glm::value_ptr(m_LightColor), 0.0f, 1.0f);


	//ImGui::Button("Phong");
	ImGui::End();

}

void PointLightShadow::RenderScene(Engine::Ref<Engine::Shader> shader, bool withCamera)
{
	shader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(shader)->SetInt("u_Blinn", m_Blinn);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(shader)->SetInt("u_Gamma", m_Gamma);

	std::dynamic_pointer_cast<Engine::OpenGLShader>(shader)->SetFloat3("u_LightPos", m_LightPos);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(shader)->SetFloat3("u_LightColor", m_LightColor);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(5.0f));

	m_BoxGeometry->SetTransform(model);
	Engine::RenderCommand::DisableCullFace();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(shader)->SetInt("u_ReverseNormals", 1);
	m_BoxMesh->Submit(shader);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(shader)->SetInt("u_ReverseNormals", 0);
	Engine::RenderCommand::EnableCullFace();
	//m_PlaneMesh->Submit(m_Shader);
	m_BoxGeometry->SetTransform(glm::mat4(1.0));

	for (uint32_t i = 0; i < 5; i++)
	{
		m_BoxGeometry->SetTransform(m_BoxTransforms[i]);
		m_BoxMesh->Submit(shader, withCamera);
	}

}
