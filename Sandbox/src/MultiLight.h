#pragma once

#include "Engine.h"

class MuitLightLayer : public Engine::Layer
{
public:
	MuitLightLayer();

	void OnUpdate(Engine::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Engine::Event& e) override;

private:
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::VertexArray> m_VertexArray;
	Engine::Ref<Engine::Texture2D> m_Texture;
	Engine::PerspectiveCameraController m_CameraController;

	glm::vec3 m_CubePositions[10] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	
	float m_CutOff;
private:
	struct Material
	{
		glm::vec3 ambient;
		uint32_t diffuse;
		uint16_t specular;
		float shininess;

		Material(const glm::vec3& ambient = glm::vec3(0.1f), 
			const uint32_t diffuse = 0, 
			const uint32_t specluar = 1, 
			const float shininess = 32.0f)
			: ambient(ambient),
			  diffuse(diffuse),
			  specular(specluar),
			  shininess(shininess)
		{
		}
	};

	struct DirectLight
	{
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		DirectLight(const glm::vec3& direction = glm::vec3(-0.2f, -1.0f, -0.3f),
			const glm::vec3& ambient  = glm::vec3(0.2f), 
			const glm::vec3& diffuse  = glm::vec3(0.3f), 
			const glm::vec3& specluar = glm::vec3(0.6f))
			: direction(direction),
			  ambient(ambient),
			  diffuse(diffuse),
			  specular(specluar)
		{
		}

	};

	struct SpotLight
	{
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float constant;
		float linear;
		float quadratic;

		float cutoff;
		float outerCutoff;

		SpotLight(const glm::vec3& direction = glm::vec3(-0.2f, -1.0f, -0.3f),
			const glm::vec3& ambient  = glm::vec3(0.0f), 
			const glm::vec3& diffuse  = glm::vec3(0.5f), 
			const glm::vec3& specluar = glm::vec3(1.0f),
			float constant = 1,
			float linear = 0.09f,
			float quadratic = 0.032f,
			float cutoff = 12.5f,
			float outerCutoff = 15.0f)
			: direction(direction),
			  ambient(ambient),
			  diffuse(diffuse),
			  specular(specluar),
			  constant(constant),
			  linear(linear),
			  quadratic(quadratic),
			  cutoff(cutoff),
			  outerCutoff(outerCutoff)
		{
		}

	};


	Engine::Ref<Material> m_Material; 
	Engine::Ref<DirectLight> m_DirectLight; 
	Engine::Ref<SpotLight> m_SpotLight; 
};
