#include "Engine.h"
#include "Engine/Core/EntryPoint.h"

#include "ExampleLayer.h"
#include "Sandbox3D.h"

class SandBox : public Engine::Application
{
public:
	SandBox()
	{
		PushLayer(new Sandbox3D());
	}
	~SandBox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new SandBox();
}