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

	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/Bloom.vert", "assets/shaders/Bloom.frag");
}

void Blooming::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::SetClearColor({ 0.0, 0.0, 0.0, 1.0 });
	Engine::RenderCommand::Clear();

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	m_ContainerMesh->Submit(m_Shader);

	for (uint32_t i = 0; i < m_BoxTransforms.size(); ++i)
	{
		m_BoxGeometry->SetTransform(m_BoxTransforms[i]);
		m_BoxMesh->Submit(m_Shader);
	}

	Engine::Renderer::EndScene();
}

void Blooming::OnEvent(Engine::Event& e)
{
}

void Blooming::OnImGuiRender()
{
}
