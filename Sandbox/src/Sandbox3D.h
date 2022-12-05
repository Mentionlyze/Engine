#pragma once
#include "Engine.h"

class Sandbox3D : public Engine::Layer
{
public:
	Sandbox3D();

	void OnUpdate(Engine::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Engine::Event& e) override;

private:
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::VertexArray> m_VertexArray;
	Engine::Ref<Engine::Texture2D> m_Texture;
	Engine::PerspectiveCameraController m_CameraController;
	glm::vec3 m_LightPos;
	glm::vec3 m_LightColor;
};

