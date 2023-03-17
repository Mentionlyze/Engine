#include "Engine.h"

class BlinnPhong : public Engine::Layer
{
public:
	BlinnPhong();

	void OnUpdate(Engine::Timestep ts) override;
	void OnEvent(Engine::Event& e) override;
	void OnImGuiRender() override;

private:
	Engine::PerspectiveCameraController m_CameraController;
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::Mesh> m_Mesh;
	Engine::Ref<Engine::ModelTexture> m_ModelTexture;
	Engine::Ref<Engine::Geometry> m_PlaneGeometry;

	glm::vec3 m_lightPositions[4] = {
	   glm::vec3(-3.0f, 0.0f, 0.0f),
	   glm::vec3(-1.0f, 0.0f, 0.0f),
	   glm::vec3(1.0f, 0.0f, 0.0f),
	   glm::vec3(3.0f, 0.0f, 0.0f)
	};
	glm::vec3 m_lightColors[4] = {
		glm::vec3(0.25),
		glm::vec3(0.50),
		glm::vec3(0.75),
		glm::vec3(1.00)
	}; 
	bool m_Blinn = false;
	bool m_Gamma = false;
};