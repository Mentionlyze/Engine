#include "ExampleLayer.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer() : Layer("Example"), m_CameraController(1.6f / 0.9f, true)
{
	m_VertexArray = Engine::VertexArray::Create();

	float vertices[4 * 5] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	};

	m_VerteBuffer = Engine::VertexBuffer::Create(vertices, sizeof(vertices));
	Engine::BufferLayout layout = {
		{ Engine::ShaderDataType::Float3, "a_Pos" },
		{ Engine::ShaderDataType::Float2, "a_TexCoord" }
	};

	m_VerteBuffer->SetLayout(layout);

	m_VertexArray->AddVertexBuffer(m_VerteBuffer);

	unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };

	m_IndexBuffer = Engine::IndexBuffer::Create(indices, (sizeof(indices) / sizeof(uint32_t)));

	m_VertexArray->SetIndexBuffer(m_IndexBuffer);

	std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;
			layout(location = 1) in vec2 a_TexCoord;
			
			out vec2 v_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0f);
				v_TexCoord = a_TexCoord;
			}
		)";

	std::string fragmentSrc = R"(
			#version 330 core
		
			in vec2 v_TexCoord;	
			out vec4 color;	
			uniform vec3 u_Color;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";


	auto textureShader = Engine::Renderer::GetShaderLibrary()->Load("texture_shader", vertexSrc, fragmentSrc);
	textureShader->Bind();

	m_Texture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");
	m_LogoTexture = Engine::Texture2D::Create("assets/textures/ChernoLogo.png");

	std::dynamic_pointer_cast<Engine::OpenGLShader>(textureShader)->SetInt("u_Texture", 0);

	m_QubeShader = Engine::Shader::Create("assets/shaders/Qube.vert", "assets/shaders/Qube.frag");
}

void ExampleLayer::OnUpdate(Engine::Timestep ts)
{

	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Engine::RenderCommand::Clear();

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	m_QubeShader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_QubeShader)->setFloat3("u_Color", m_SquareColor);

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	for (unsigned int r = 0; r < 20; r++)
	{
		for (unsigned int c = 0; c < 20; c++)
		{
			glm::vec3 pos = glm::vec3(0.11f * c, 0.11f * r, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Engine::Renderer::Submit(m_VertexArray, m_QubeShader, transform);
		}
	}

	auto textureShader = Engine::Renderer::GetShaderLibrary()->Get("texture_shader");

	m_Texture->Bind();
	Engine::Renderer::Submit(m_VertexArray, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	m_LogoTexture->Bind();
	Engine::Renderer::Submit(m_VertexArray, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Engine::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("start");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void ExampleLayer::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
}
