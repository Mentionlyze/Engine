#include "HDR.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

HDR::HDR() : Layer("HDR"), m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 1000.0f)
{


	m_BoxGeometry = Engine::Geometry::CreateBox(glm::scale(glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 25.0f)), glm::vec3(2.5f, 2.5f, 27.5f)));
	m_ModelTexture = Engine::ModelTexture::Create();
	m_ModelTexture->AddMaterialTexture("assets/textures/wood.png", "texture_diffuse");

	m_BoxMesh = Engine::Mesh::Create(m_BoxGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);

	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/Lighting.vert", "assets/shaders/Lighting.frag");

	m_LightGeometry = Engine::Geometry::CreateBox(glm::mat4(1.0f));
	m_LightMesh = Engine::Mesh::Create(m_LightGeometry, {});

	m_LightShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/LightCube.vert", "assets/shaders/LightCube.frag");
	m_HDRShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/HDR.vert", "assets/shaders/HDR.frag");

	m_FrameBuffer = Engine::FrameBuffer::Create();
	m_ColorTexture = Engine::TextureColorBuffer::Create(1600, 900);
	m_RenderBuffer = Engine::RenderBuffer::Create(1600, 900);

	m_FrameBuffer->SetTexture(std::dynamic_pointer_cast<Engine::OpenGLTextureColorBuffer>(m_ColorTexture)->GetRendererID(), GL_COLOR_ATTACHMENT0);

	uint32_t rendererBufferID = std::dynamic_pointer_cast<Engine::OpenGLRenderBuffer>(m_RenderBuffer)->GetRendererID();
	std::dynamic_pointer_cast<Engine::OpenGLFrameBuffer>(m_FrameBuffer)->SetRenderBuffer(rendererBufferID);
	m_FrameBuffer->Unbind();

	float vertices[] = {
		// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	m_VertexArray = Engine::VertexArray::Create();
	auto vertexBuffer = Engine::VertexBuffer::Create(vertices, sizeof(vertices));
	Engine::BufferLayout layout = {
		{ Engine::ShaderDataType::Float3, "a_Pos" },
		{ Engine::ShaderDataType::Float2, "a_TexCoords" },
	};
	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[] = {
		0, 1, 2,
		2, 3, 1
	};
	auto indexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(indexBuffer);
}

void HDR::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::SetClearColor({ 0.0, 0.0, 0.0, 1.0 });
	Engine::RenderCommand::Clear();

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	m_FrameBuffer->Bind();
	Engine::RenderCommand::Clear();


	m_Shader->Bind();
	for (uint32_t i = 0; i < m_LightPositions.size(); ++i)
	{
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
	}

	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetInt("u_InverseNormals", 1);
	m_BoxMesh->Submit(m_Shader);

	m_LightShader->Bind();
	for (uint32_t i = 0; i < m_LightPositions.size(); ++i)
	{
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_LightShader)->SetFloat3("u_LightColor", m_LightColors[i]);
		m_LightGeometry->SetTransform(glm::scale(glm::translate(glm::mat4(1.0), m_LightPositions[i]), glm::vec3(0.2)));
		m_LightMesh->Submit(m_LightShader);
	}

	m_FrameBuffer->Unbind();
	Engine::RenderCommand::Clear();


	m_HDRShader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_HDRShader)->SetInt("u_HDR", m_HDR);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_HDRShader)->SetFloat("u_Exposure", m_Exposure);
	m_ColorTexture->Bind();

	Engine::Renderer::Submit(m_VertexArray, m_HDRShader, glm::mat4(1.0f));


	Engine::Renderer::EndScene();
}

void HDR::OnEvent(Engine::Event& e)
{
}

void HDR::OnImGuiRender()
{
	ImGui::Begin("HDR");

	if (ImGui::Button("Enable HDR"))
		m_HDR = true;
	if (ImGui::Button("Disable HDR"))
		m_HDR = false;

	ImGui::SliderFloat("Exposure", &m_Exposure, 0.0, 10.0);

	ImGui::End();
}
