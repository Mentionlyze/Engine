#include "Sandbox3D.h"

Sandbox3D::Sandbox3D()
{
}

void Sandbox3D::OnUpdate(Engine::Timestep ts)
{
	Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Engine::RenderCommand::Clear();
}

void Sandbox3D::OnImGuiRender()
{
}

void Sandbox3D::OnEvent(Engine::Event& e)
{
}
