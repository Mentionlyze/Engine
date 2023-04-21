#pragma once
#include "Engine.h"

class FrameBuffer : public Engine::Layer
{
public:
	FrameBuffer();

	void OnUpdate(Engine::Timestep ts) override;
	void OnEvent(Engine::Event& e) override;
	void OnImGuiRender() override;

private:
	Engine::PerspectiveCameraController m_CameraController;
	Engine::Ref<Engine::Geometry> m_Geometry;
	Engine::Ref<Engine::Mesh> m_Mesh;
	
	Engine::Ref<Engine::FrameBuffer> m_FirstFrameBuffer;
	Engine::Ref<Engine::RenderBuffer> m_FirstRenderBuffer;
	Engine::Ref<Engine::TextureColorBuffer> m_FirstColorBuffer;

	Engine::Ref<Engine::FrameBuffer> m_SecondFrameBuffer;
	Engine::Ref<Engine::RenderBuffer> m_SecondRenderBuffer;
	Engine::Ref<Engine::TextureColorBuffer> m_SecondColorBuffer;

	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::Shader> m_ProcessShader;


	Engine::Ref<Engine::VertexArray> m_VertexArray;
};

