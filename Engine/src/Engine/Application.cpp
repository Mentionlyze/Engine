#include "Precompile.h"
#include "Application.h"
#include "Engine/Log.h"

#include <glad/glad.h>

#include "Engine/Input.h"

namespace Engine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{
		ENGINE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create()); 
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		m_VerteBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Pos" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		m_VerteBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(m_VerteBuffer);
	
		unsigned int indices[3] = { 0, 1, 2 };

		m_IndexBuffer.reset(IndexBuffer::Create(indices, (sizeof(indices) / sizeof(uint32_t))));

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;
			layout(location = 1) in vec4 a_Color;
			
			out vec4 v_Color;

			void main()
			{
				gl_Position = vec4(a_Pos, 1.0f);
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

		m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));
	}
	Application::~Application() {}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1, 0.1, 0.1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			m_VertexArray->Bind();

			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//ENGINE_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverLay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		
		return true;
	}
}
