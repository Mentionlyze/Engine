#pragma once
#include "Engine.h"

class NormalMapping : public Engine::Layer
{
public:
	NormalMapping();

	void OnUpdate(Engine::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Engine::Event& e) override;

private:
	Engine::PerspectiveCameraController m_CameraController;
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::VertexArray> m_VertexArray;
	Engine::Ref<Engine::Texture2D> m_Texutre;
	Engine::Ref<Engine::Texture2D> m_NormalTexture;

	glm::vec3 m_LightPos = glm::vec3(0.5f, 1.0f, 0.3f);
};

