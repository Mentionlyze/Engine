#include "Engine.h"
#include "Engine/Core/EntryPoint.h"

#include "ExampleLayer.h"
#include "Sandbox3D.h"
#include "MultiLight.h"
#include "ModelLoad.h"
#include "DepthStencil.h"
#include "Skybox.h"
#include "Planet.h"
#include "BlinnPhong.h"
#include "ShadowMap.h"
#include "PointLightShadow.h"
#include "DepthCubMap.h"
#include "NormalMapping.h"
#include "HDR.h"
#include "Blooming.h"

class SandBox : public Engine::Application
{
public:
	SandBox()
	{
		//PushLayer(new DepthCubMap());
		//PushLayer(new PointLightShadow());
		//PushLayer(new ShadowMap());
		//PushLayer(new Skybox());
		//PushLayer(new NormalMapping());
		//PushLayer(new HDR());
		PushLayer(new Blooming());
	}
	~SandBox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new SandBox();
}