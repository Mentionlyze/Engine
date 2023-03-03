#include "Engine.h"
#include "Engine/Core/EntryPoint.h"

#include "ExampleLayer.h"
#include "Sandbox3D.h"
#include "MultiLight.h"
#include "ModelLoad.h"
#include "DepthStencil.h"

class SandBox : public Engine::Application
{
public:
	SandBox()
	{
		PushLayer(new DepthStencil());
	}
	~SandBox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new SandBox();
}