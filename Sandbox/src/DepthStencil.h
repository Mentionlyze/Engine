#pragma once
#include "Engine.h"
#include "imgui/imgui.h"

class DepthStencil : public Engine::Layer
{
public:
	DepthStencil();

	void OnUpdate(Engine::Timestep ts) override;

	void OnImGuiRender() override;

	void OnEvent(Engine::Event& event) override;

private:
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::Shader> m_BorderShader;
	Engine::Ref<Engine::Shader> m_TransparentShader;
	Engine::Ref<Engine::VertexArray> m_VertexArray;
	Engine::Ref<Engine::VertexArray> m_PlaneVertexArray;
	Engine::Ref<Engine::VertexArray> m_TransparentVertexArray;
	Engine::Ref<Engine::Texture2D> m_CubeTexture;
	Engine::Ref<Engine::Texture2D> m_FloorTexture;
	Engine::Ref<Engine::Texture2D> m_TransparentTexture;
	Engine::PerspectiveCameraController m_CameraController;

	std::vector<glm::vec3> m_Vegetation = {
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f),
	};
};

