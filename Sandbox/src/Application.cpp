#include <Engine.h>
#include <imgui/imgui.h>
#include <imgui/imgui.h>

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//ENGINE_INFO("ExampleLayer::Update");

		if (Engine::Input::IsKeyPressed(Engine::Key::Tab))
			ENGINE_INFO("Tab is Pressed!");
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("start");
		ImGui::Text("Hello UI");
		ImGui::End();
	}

	void OnEvent(Engine::Event& event) override
	{
		if (event.GetEventType() == Engine::EventType::KeyPressed)
		{
			Engine::KeyPressedEvent& e = (Engine::KeyPressedEvent&)event;
			ENGINE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

private:

};


class SandBox : public Engine::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
	}
	~SandBox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new SandBox();
}