#pragma once

#include "Engine.h"

class ModelLoad : public Engine::Layer
{
public:
	ModelLoad();

	void OnUpdate(Engine::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Engine::Event& e) override;

private:
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::Model> m_Model;
	Engine::PerspectiveCameraController m_CameraController;
};

