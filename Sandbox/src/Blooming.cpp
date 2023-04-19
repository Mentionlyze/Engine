#include "Blooming.h"

Blooming::Blooming() : Layer("Blooming"), m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 1000.0f)
{
	m_BoxGeometry = Engine::Geometry::CreateBox(glm::mat4(1.0f));
	m_ModelTexture = Engine::ModelTexture::Create();
	m_ModelTexture->AddMaterialTexture("assets/textures/container2.png", "texture_diffuse");
	m_BoxMesh = Engine::Mesh::Create(m_BoxGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);

	m_ContainerGeometry = Engine::Geometry::CreateBox(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), glm::vec3(12.5f, 0.5f, 12.5f)));
	m_ContainerModelTexture = Engine::ModelTexture::Create();
	m_ContainerModelTexture->AddMaterialTexture("assets/textures/wood.png", "texture_diffuse");
	m_ContainerMesh = Engine::Mesh::Create(m_ContainerGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ContainerModelTexture)->m_Texuters);

	m_LightGeometry = Engine::Geometry::CreateBox(glm::mat4(1.0f));
	m_LightMesh = Engine::Mesh::Create(m_LightGeometry, {});

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

	

	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/Bloom.vert", "assets/shaders/Bloom.frag");

	m_LightShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/BloomBox.vert", "assets/shaders/BloomBox.frag");

	m_BlurShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/Blur.vert", "assets/shaders/Blur.frag");

	m_BloomShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/BloomFinal.vert", "assets/shaders/BloomFinal.frag");

	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_BloomShader)->SetInt("scene", 0);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_BloomShader)->SetInt("bloomBlur", 1);

	m_HDRRenderBuffer = Engine::RenderBuffer::Create(1600, 900);
	m_HDRFrameBuffer = Engine::FrameBuffer::Create();

	for (uint32_t i = 0; i < 2; ++i)
	{
		m_HDRColorBuffers[i] = Engine::TextureColorBuffer::Create(1600, 900);
		m_HDRFrameBuffer->SetTexture(
			std::dynamic_pointer_cast<Engine::OpenGLTextureColorBuffer>(m_HDRColorBuffers[i])->GetRendererID(), 
			GL_COLOR_ATTACHMENT0 + i
		);
	}
	std::dynamic_pointer_cast<Engine::OpenGLFrameBuffer>(m_HDRFrameBuffer)->SetRenderBuffer(
		std::dynamic_pointer_cast<Engine::OpenGLRenderBuffer>(m_HDRRenderBuffer)->GetRendererID()
	);

	uint32_t attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	m_HDRFrameBuffer->Unbind();

	for (uint32_t i = 0; i < 2; ++i)
	{
		m_BloomRenderBuffers[i] = Engine::RenderBuffer::Create(1600, 900);
		m_BloomFrameBuffers[i] = Engine::FrameBuffer::Create();
		m_BloomColorBuffers[i] = Engine::TextureColorBuffer::Create(1600, 900);
		m_BloomFrameBuffers[i]->SetTexture(
			std::dynamic_pointer_cast<Engine::OpenGLTextureColorBuffer>(m_BloomColorBuffers[i])->GetRendererID(),
			GL_COLOR_ATTACHMENT0
		);
		std::dynamic_pointer_cast<Engine::OpenGLFrameBuffer>(m_BloomFrameBuffers[i])->SetRenderBuffer(
			std::dynamic_pointer_cast<Engine::OpenGLRenderBuffer>(m_BloomRenderBuffers[i])->GetRendererID()
		);
	}

	m_HDRFrameBuffer->Unbind();
}

void Blooming::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::SetClearColor({ 0.0, 0.0, 0.0, 1.0 });
	Engine::RenderCommand::Clear();

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	m_HDRFrameBuffer->Bind();
	Engine::RenderCommand::Clear();

	m_Shader->Bind();
	for (uint32_t i = 0; i < m_LightPositions.size(); ++i)
	{
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
	}

	m_ContainerMesh->Submit(m_Shader);


	m_BoxGeometry->SetTransform(glm::mat4(1.0f));

	for (uint32_t i = 0; i < m_BoxTransforms.size(); ++i)
	{
		m_BoxGeometry->SetTransform(m_BoxTransforms[i]);
		m_BoxMesh->Submit(m_Shader);
	}

	m_LightShader->Bind();
	for (uint32_t i = 0; i < m_LightPositions.size(); ++i)
	{
		m_BoxGeometry->SetTransform(glm::scale(glm::translate(glm::mat4(1.0), m_LightPositions[i]), glm::vec3(0.25f)));
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_LightShader)->SetFloat3("u_LightColor", m_LightColors[i]);
		m_BoxMesh->Submit(m_LightShader);
	}

	m_HDRFrameBuffer->Unbind();

	bool horizontal = true, first_iteration = true;
	uint32_t amount = 10;
	m_BlurShader->Bind();
	for (uint32_t i = 0; i < amount; ++i)
	{ 
		uint32_t index = horizontal ? 1 : 0;

		m_BloomFrameBuffers[index]->Bind();
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_BlurShader)->SetInt("horizontal", horizontal);
		if (first_iteration)
			m_HDRColorBuffers[1]->Bind();
		else
			m_BloomColorBuffers[index]->Bind();

		Engine::Renderer::Submit(m_VertexArray, m_BlurShader, glm::mat4(1.0));
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	m_HDRFrameBuffer->Unbind();
	Engine::RenderCommand::Clear();

	m_BloomShader->Bind();
	m_HDRColorBuffers[0]->Bind();
	m_BloomColorBuffers[0]->Bind(1);

	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_BloomShader)->SetInt("bloom", true);
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_BloomShader)->SetFloat("exposure", 1.0);

	Engine::Renderer::Submit(m_VertexArray, m_BloomShader, glm::mat4(1.0));

	Engine::Renderer::EndScene();
}

void Blooming::OnEvent(Engine::Event& e)
{
}

void Blooming::OnImGuiRender()
{
}
