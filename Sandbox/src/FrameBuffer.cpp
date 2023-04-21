#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() : Layer("FrameBuffer"), m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 1000.0f)
{
	m_Geometry = Engine::Geometry::CreateBox(glm::mat4(1.0f));
	m_Mesh = Engine::Mesh::Create(m_Geometry, {});

	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/ColorCube.vert", "assets/shaders/ColorCube.frag");
	
	m_ProcessShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/ColorBuffer.vert", "assets/shaders/ColorBuffer.frag");

	m_FirstFrameBuffer = Engine::FrameBuffer::Create();
	m_FirstRenderBuffer = Engine::RenderBuffer::Create(1600, 900);
	m_FirstColorBuffer = Engine::TextureColorBuffer::Create(1600, 900);

	m_FirstFrameBuffer->SetTexture(std::dynamic_pointer_cast<Engine::OpenGLTextureColorBuffer>(m_FirstColorBuffer)->GetRendererID(), GL_COLOR_ATTACHMENT0);
	std::dynamic_pointer_cast<Engine::OpenGLFrameBuffer>(m_FirstFrameBuffer)->SetRenderBuffer(
		std::dynamic_pointer_cast<Engine::OpenGLRenderBuffer>(m_FirstRenderBuffer)->GetRendererID()
	);

	m_FirstFrameBuffer->Unbind();

	m_SecondFrameBuffer = Engine::FrameBuffer::Create();
	m_SecondRenderBuffer = Engine::RenderBuffer::Create(1600, 900);
	m_SecondColorBuffer = Engine::TextureColorBuffer::Create(1600, 900);

	m_SecondFrameBuffer->SetTexture(std::dynamic_pointer_cast<Engine::OpenGLTextureColorBuffer>(m_SecondColorBuffer)->GetRendererID(), GL_COLOR_ATTACHMENT0);
	std::dynamic_pointer_cast<Engine::OpenGLFrameBuffer>(m_SecondFrameBuffer)->SetRenderBuffer(
		std::dynamic_pointer_cast<Engine::OpenGLRenderBuffer>(m_SecondRenderBuffer)->GetRendererID()
	);

	m_FirstFrameBuffer->Unbind();

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

void FrameBuffer::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::SetClearColor({ 0.0, 0.0, 0.0, 1.0 });
	Engine::RenderCommand::Clear();

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	m_FirstFrameBuffer->Bind();
	Engine::RenderCommand::Clear();

	m_Mesh->Submit(m_Shader);

	m_FirstFrameBuffer->Unbind();

	m_SecondFrameBuffer->Bind();
	//Engine::RenderCommand::Clear();

	m_ProcessShader->Bind();
	m_FirstColorBuffer->Bind();
	Engine::Renderer::Submit(m_VertexArray, m_ProcessShader, glm::mat4(1.0));
	
	m_SecondFrameBuffer->Unbind();
	Engine::RenderCommand::Clear();

	m_ProcessShader->Bind();
	m_FirstColorBuffer->Unbind();
	m_SecondColorBuffer->Bind();
	Engine::Renderer::Submit(m_VertexArray, m_ProcessShader, glm::mat4(1.0));

	Engine::Renderer::EndScene();
}

void FrameBuffer::OnEvent(Engine::Event& e)
{
}

void FrameBuffer::OnImGuiRender()
{
}
