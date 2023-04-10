#include "NormalMapping.h"

NormalMapping::NormalMapping() : Layer("Normal Mapping"),
	m_CameraController(glm::radians(75.0f), 1.6f / 0.9f, 0.1f, 100.0f)
{
	m_VertexArray = Engine::VertexArray::Create();



	//auto vertexBuffer = Engine::VertexBuffer::Create();
	//auto indexBuffer = Engine::IndexBuffer::Create();

	m_Texutre = Engine::Texture2D::Create("assets/textures/brickwall.jpg");
	m_NormalTexture = Engine::Texture2D::Create("assets/textures/brickwall_normal.jpg");
}

void NormalMapping::OnUpdate(Engine::Timestep ts)
{
}

void NormalMapping::OnImGuiRender()
{
}

void NormalMapping::OnEvent(Engine::Event& e)
{
}
