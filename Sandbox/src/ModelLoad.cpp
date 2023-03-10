#include "ModelLoad.h"
#include "glm/gtc/matrix_transform.hpp"

ModelLoad::ModelLoad() : m_CameraController(45.0f, 1.6f/0.9f, 0.1f, 100.0f)
{
	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/ModelLoad.vert", "assets/shaders/ModelLoad.frag");
	m_VisualShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/Visualization.vert", "assets/shaders/Visualization.frag", "assets/shaders/Visualization.geom");
	m_Model = Engine::Renderer::GetModelLibrary()->Load("assets/objects/nanosuit/nanosuit.obj");
}

void ModelLoad::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Engine::RenderCommand::Clear();

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	m_Model->Submit(m_Shader, glm::mat4(1.0f));
	m_Model->Submit(m_VisualShader, glm::mat4(1.0f));

	Engine::Renderer::EndScene();
}

void ModelLoad::OnImGuiRender()
{
}

void ModelLoad::OnEvent(Engine::Event& e)
{
}
