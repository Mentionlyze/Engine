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
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::Shader> m_DepthShader;
	Engine::Ref<Engine::Texture2D> m_WoodTexture;

	uint32_t m_DepthCubeMapTexture;
	uint32_t m_FrameBuffer;

	glm::vec3 m_LightPos = glm::vec3(0.0f, 0.0f, 0.0f);

	uint32_t cubeVAO = 0;
	uint32_t cubeVBO = 0;

	void RenderScene(const Engine::Ref<Engine::Shader> shader);
	void RenderCube();
};

