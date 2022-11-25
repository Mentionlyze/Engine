#include <Engine.h>
#include <imgui/imgui.h>

#include <glm/glm.hpp>

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		m_VertexArray.reset(Engine::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		m_VerteBuffer.reset(Engine::VertexBuffer::Create(vertices, sizeof(vertices)));
		Engine::BufferLayout layout = {
			{ Engine::ShaderDataType::Float3, "a_Pos" },
			{ Engine::ShaderDataType::Float4, "a_Color" }
		};

		m_VerteBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(m_VerteBuffer);
	
		unsigned int indices[3] = { 0, 1, 2 };

		m_IndexBuffer.reset(Engine::IndexBuffer::Create(indices, (sizeof(indices) / sizeof(uint32_t))));

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;
			layout(location = 1) in vec4 a_Color;
			
			out vec4 v_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0f);
				v_Color = a_Color;
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
		
			in vec4 v_Color;	
			out vec4 color;	

			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader.reset(Engine::Shader::Create(vertexSrc, fragmentSrc));

	}

	void OnUpdate() override
	{
		Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Engine::RenderCommand::Clear();

		Engine::Renderer::BeginScene(m_Camera);
		Engine::Renderer::Submit(m_VertexArray, m_Shader, glm::mat4(1.0f));
		Engine::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("start");
		ImGui::Text("Hello UI");
		ImGui::End();
	}

	void OnEvent(Engine::Event& event) override
	{
		if (event.GetEventType() == Engine::EventType::KeyPressed)
		{
			Engine::KeyPressedEvent& e = (Engine::KeyPressedEvent&)event;
			ENGINE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

private:
	std::shared_ptr<Engine::Shader> m_Shader;
	std::shared_ptr<Engine::VertexArray> m_VertexArray;
	std::shared_ptr<Engine::VertexBuffer> m_VerteBuffer;
	std::shared_ptr<Engine::IndexBuffer> m_IndexBuffer;
	Engine::OrthoGraphicsCamera m_Camera;
};


class SandBox : public Engine::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
	}
	~SandBox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new SandBox();
}