#include "ModelLoad.h"

ModelLoad::ModelLoad() : m_CameraController(45.0f, 1.6f /0.9f, 0.1f, 100.f)
{
	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/ModelLoad.vert", "assets/shaders/ModelLoad.frag");
	m_Model = Engine::Renderer::GetModelLibrary()->Load("assets/objects/backpack/backpack.obj");
}

void ModelLoad::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Engine::RenderCommand::Clear();

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	m_Model->Submit(m_Shader, glm::mat4(1.0f));

	Engine::Renderer::EndScene();
}

void ModelLoad::OnImGuiRender()
{
}

void ModelLoad::OnEvent(Engine::Event& e)
{
}
