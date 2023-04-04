#pragma once
#include "Engine.h"
class DepthCubMap : public Engine::Layer
{
public:
	DepthCubMap();

	void OnUpdate(Engine::Timestep ts) override;

	void OnEvent(Engine::Event& e) override;

	void OnImGuiRender() override;
private:
	Engine::PerspectiveCameraController m_CameraController;

	uint32_t cubeVAO = 0;
	uint32_t cubeVBO = 0;
};

