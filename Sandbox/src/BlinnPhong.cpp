#include "BlinnPhong.h"

BlinnPhong::BlinnPhong() : Layer("Blinn Phong") , m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 100.f)
{
	m_PlaneGeometry = Engine::Geometry::CreatePlane(glm::mat4(1.0f));
	m_ModelTexture = Engine::ModelTexture::Create();
	m_ModelTexture->AddMaterialTexture("assets/textures/wood.png", "texture_diffuse");
	m_Mesh = Engine::Mesh::Create(m_PlaneGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);
	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/ModelLoad.vert", "assets/shaders/SingleTexture.frag");
	m_Shader->Bind();
}

void BlinnPhong::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 0.0, 0.0, 0.0, 1.0 });

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());
	
	m_Mesh->Submit(m_Shader);

	Engine::Renderer::EndScene();

}

void BlinnPhong::OnEvent(Engine::Event& e)
{
}

void BlinnPhong::OnImGuiRender()
{
}
