#include "Planet.h"

Planet::Planet() : m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 100.f)
{
	m_Model = Engine::Renderer::GetModelLibrary()->Load("assets/objects/planet/planet.obj");
	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/ModelLoad.vert", "assets/shaders/ModelLoad.frag");
	m_AsteroidShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/Asteroid.vert", "assets/shaders/Asteroid.frag");

}

void Planet::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

    Engine::RenderCommand::SetClearColor({ 0.0, 0.0, 0.0, 1.0 });
	Engine::RenderCommand::Clear();

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	m_Model->Submit(m_Shader, glm::mat4(1.0f));

	Engine::Renderer::EndScene();
}

void Planet::OnEvent(Engine::Event& e)
{
}

void Planet::OnImGuiRender()
{
}

