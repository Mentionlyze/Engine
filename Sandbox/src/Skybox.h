#pragma once
#include "Engine.h"

class Skybox : public Engine::Layer
{
public:
	Skybox();

	void OnUpdate(Engine::Timestep ts) override;
	void OnEvent(Engine::Event &e ) override;
	void OnImGuiRender() override;

private:
	Engine::Ref<Engine::VertexArray> m_CubeVertexArray;
	Engine::Ref<Engine::VertexArray> m_SkyboxVertexArray;
	Engine::Ref<Engine::Shader> m_SkyboxShader;
	Engine::Ref<Engine::Shader> m_CubeShader;
	Engine::Ref<Engine::Texture2D> m_CubeTexture;
	Engine::Ref<Engine::CubeMap> m_SkyboxMap;
	Engine::PerspectiveCameraController m_CameraController;
};

