#pragma once
#include "Engine.h"

class Planet : public Engine::Layer
{
public:
	Planet();

	void OnUpdate(Engine::Timestep ts) override;
	void OnEvent(Engine::Event& e) override;
	void OnImGuiRender() override;
private:
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::Shader> m_AsteroidShader;
	Engine::Ref<Engine::Model> m_Model;
	Engine::PerspectiveCameraController m_CameraController;

	uint32_t amount;
	glm::mat4* m_ModelMatrixs = new glm::mat4[amount];
};

