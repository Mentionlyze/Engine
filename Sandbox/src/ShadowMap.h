#pragma once
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>

class ShadowMap : public Engine::Layer
{
public:
	ShadowMap();

	void OnUpdate(Engine::Timestep ts) override;
	void OnEvent(Engine::Event& e) override;
	void OnImGuiRender() override;

	void RenderScene(Engine::Ref<Engine::Shader> shader, bool withCamera = true);

private:
	Engine::PerspectiveCameraController m_CameraController;
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::Shader> m_LightShader;
	Engine::Ref<Engine::Shader> m_DepthShader;

	Engine::Ref<Engine::Geometry> m_LightGeometry;
	Engine::Ref<Engine::Geometry> m_PlaneGeometry;
	Engine::Ref<Engine::Geometry> m_BoxGeometry;
	Engine::Ref<Engine::Mesh> m_LightMesh;
	Engine::Ref<Engine::Mesh> m_PlaneMesh;
	Engine::Ref<Engine::Mesh> m_BoxMesh;
	Engine::Ref<Engine::ModelTexture> m_ModelTexture;
	Engine::Ref<Engine::TextureDepthMap> m_DepthTexture;

	Engine::Ref<Engine::FrameBuffer> m_FrameBuffer;

	glm::vec3 m_LightPos = glm::vec3(0.0f, 4.0f, -1.0f);
	glm::vec3 m_LightColor = glm::vec3(1.0);

	glm::mat4 m_BoxTransforms[3] = {
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0)),
		glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 1.0)),

		glm::scale(
			glm::rotate(
				glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 2.0)),
				60.0f, 
				glm::normalize(glm::vec3(1.0, 0.0, 1.0))
			), 
			glm::vec3(0.5))
		};

	bool m_Blinn = true;
	bool m_Gamma = true;
};

