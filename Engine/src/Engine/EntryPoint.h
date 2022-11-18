#pragma once
#include "../Engine.h"

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	Engine::Log::Init();

	ENGINE_CORE_WARN("Initialize Log"); 
	int a = 5;
	ENGINE_CORE_INFO("Hello Val={0}", a);

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}

#endif //  ENGINE_PLATFORM_WINDOWS

