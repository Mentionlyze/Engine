#pragma once
#include <Engine.h>
#include <imgui/imgui.h>


class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer();
	
	void OnUpdate(Engine::Timestep ts) override;

	void OnImGuiRender() override;

	void OnEvent(Engine::Event& event) override;
	
private:
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::Shader> m_QubeShader;
	Engine::Ref<Engine::Texture2D> m_Texture;
	Engine::Ref<Engine::Texture2D> m_LogoTexture;
	Engine::Ref<Engine::VertexArray> m_VertexArray;
	Engine::Ref<Engine::VertexBuffer> m_VerteBuffer;
	Engine::Ref<Engine::IndexBuffer> m_IndexBuffer;
	Engine::OrthoGraphicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2, 0.3, 0.7 };
};

