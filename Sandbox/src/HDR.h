#pragma once

#include "Engine.h"

class HDR : public Engine::Layer
{
public:
	HDR();
	void OnUpdate(Engine::Timestep ts) override;
	void OnEvent(Engine::Event& e) override;
	void OnImGuiRender() override;

private:
	Engine::PerspectiveCameraController m_CameraController;
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::Shader> m_HDRShader;
	Engine::Ref<Engine::Shader> m_LightShader;
	Engine::Ref<Engine::Geometry> m_LightGeometry;
	Engine::Ref<Engine::Geometry> m_BoxGeometry;
	Engine::Ref<Engine::Mesh> m_LightMesh;
	Engine::Ref<Engine::Mesh> m_BoxMesh;
	Engine::Ref<Engine::ModelTexture> m_ModelTexture;

	Engine::Ref<Engine::FrameBuffer> m_FrameBuffer;
	Engine::Ref<Engine::RenderBuffer> m_RenderBuffer;
	Engine::Ref<Engine::TextureColorBuffer> m_ColorTexture;

	Engine::Ref<Engine::VertexArray> m_VertexArray;

	bool m_HDR = true;
	float m_Exposure = 5.0f;

	std::vector<glm::vec3> m_LightPositions = {
		glm::vec3(0.0f, 0.0f, 49.5f),
		glm::vec3(-1.4f, -1.9f, 9.0f),
		glm::vec3(0.0f, -1.8f, 4.0f),
		glm::vec3(0.8f, -1.7f, 6.0f),
	};
	std::vector<glm::vec3> m_LightColors = {
		glm::vec3(200.0f, 200.0f, 200.0f),
		glm::vec3(0.1f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.2f),
		glm::vec3(0.0f, 0.1f, 0.0f),
	};
};

