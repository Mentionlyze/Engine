#pragma once
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>

class PointLightShadow : public Engine::Layer
{
public:
	PointLightShadow();

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
	Engine::Ref<Engine::TextureDepthCubeMap> m_DepthTexture;

	Engine::Ref<Engine::FrameBuffer> m_FrameBuffer;

	glm::vec3 m_LightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_LightColor = glm::vec3(1.0);

	glm::mat4 m_BoxTransforms[5] = {
		glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -3.5f, 0.0)), glm::vec3(0.5f)),
		glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 1.0)), glm::vec3(0.75f)),
		glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, -1.0f, 0.0)), glm::vec3(0.5f)),
		glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 1.0f, 1.5f)), glm::vec3(0.5f)),

		glm::scale(
			glm::rotate(
				glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 2.0f, -3.0f)),
				glm::radians(60.0f),
				glm::normalize(glm::vec3(1.0, 0.0, 1.0))
			),
			glm::vec3(0.75f))
	};

	bool m_Blinn = true;
	bool m_Gamma = true;

	glm::mat4 m_ShadowProjection;
	float m_FarPlane = 1000.0;
};

