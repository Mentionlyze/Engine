#pragma once
#include "Engine.h"

class Sandbox3D : public Engine::Layer
{
public:
	Sandbox3D();

	void OnUpdate(Engine::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Engine::Event& e) override;
};

