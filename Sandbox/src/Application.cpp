#include <Engine.h>
#include <imgui/imgui.h>

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{}

	void OnUpdate() override
	{
		ENGINE_INFO("ExampleLayer::Update");
	}

	void OnEvent(Engine::Event& event) override
	{
		ENGINE_TRACE("{0}", event);
	}

private:

};


class SandBox : public Engine::Application
{
public:
	SandBox()
	{
	}
	~SandBox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new SandBox();
}