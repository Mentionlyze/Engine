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
};