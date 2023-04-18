#pragma once
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>

class Blooming : public Engine::Layer
{
public:
	Blooming();

	void OnUpdate(Engine::Timestep ts) override;
	void OnEvent(Engine::Event& e) override;
	void OnImGuiRender() override;

private:
	Engine::PerspectiveCameraController m_CameraController; 

	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::Shader> m_LightShader;
	Engine::Ref<Engine::Shader> m_BlurShader;
	Engine::Ref<Engine::Shader> m_BloomShader;

	Engine::Ref<Engine::Geometry> m_BoxGeometry;
	Engine::Ref<Engine::Geometry> m_ContainerGeometry;
	Engine::Ref<Engine::Geometry> m_LightGeometry;

	Engine::Ref<Engine::ModelTexture> m_ModelTexture;
	Engine::Ref<Engine::ModelTexture> m_ContainerModelTexture;

	Engine::Ref<Engine::Mesh> m_BoxMesh;
	Engine::Ref<Engine::Mesh> m_ContainerMesh;
	Engine::Ref<Engine::Mesh> m_LightMesh;

	std::vector<glm::mat4> m_BoxTransforms = {
		glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0)), glm::vec3(0.5f)),
		glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 1.0)), glm::vec3(0.5f)),
		glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 2.0)), glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),
		glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 2.0)), glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))), glm::vec3(1.25f)),
		glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 2.0)), glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),
		glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0)), glm::vec3(0.5f)),
	};
};

