#pragma once
#include "Engine.h"

class Sandbox3D : public Engine::Layer
{
public:
	Sandbox3D();

	void OnUpdate(Engine::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Engine::Event& e) override;

private:
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::VertexArray> m_VertexArray;
	Engine::Ref<Engine::Texture2D> m_Texture;
	Engine::PerspectiveCameraController m_CameraController;
	glm::vec3 m_LightPos;
	glm::vec3 m_LightColor;

	
private:
	struct Material
	{
		glm::vec3 ambient;
		uint32_t diffuse;
		uint16_t specular;
		float shininess;

		Material(const glm::vec3& ambient = glm::vec3(0.3f), 
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

	struct Light
	{
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		Light(const glm::vec3& position = glm::vec3(1.2f, 2.0f, 2.0f),
			const glm::vec3& ambient = glm::vec3(0.2f),
			const glm::vec3& diffuse = glm::vec3(0.5f),
			const glm::vec3& specular = glm::vec3(1.0))
			: position(position),
			  ambient(ambient),
			  diffuse(diffuse),
			  specular(specular)
		{}
	};

	Engine::Ref<Material> m_Material; 
	Engine::Ref<Light> m_Light;
};

