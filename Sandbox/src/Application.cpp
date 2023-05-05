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
#include "FrameBuffer.h"
#include "PBR.h"
#include "PBR_Diffuse.h"
#include "PBR_Specular.h"
#include "PBR_SpecularTexture.h"

class SandBox : public Engine::Application
{
public:
	SandBox()
	{
		//PushLayer(new Sandbox3D());
		//PushLayer(new DepthCubMap());
		//PushLayer(new DepthStencil());
		//PushLayer(new PointLightShadow());
		//PushLayer(new ShadowMap());
		//PushLayer(new Skybox());
		PushLayer(new NormalMapping());
		//PushLayer(new HDR());
		//PushLayer(new Blooming());
		//PushLayer(new FrameBuffer());
		//PushLayer(new PBR());
		//PushLayer(new PBR_Diffuse());
		//PushLayer(new PBR_Specular());
		//PushLayer(new PBR_SpecularTexture());
	}
	~SandBox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new SandBox();
}