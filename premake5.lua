workspace "Engine"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"

include "Engine/vendor/GLFW"

project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "Precompile.h"
	pchsource "Engine/src/Precompile.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib",
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "On"
		systemversion "10.0.19041.0"

		defines
		{
			"ENGINE_PLATFORM_WINDOWS",
			"ENGINE_BUILD_DLL",
			"ENGINE_ENABLE_ASSERTS"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "ENGINE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		optimize"On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Engine/vendor/spdlog/include",
		"Engine/src"
	}

	links
	{
		"Engine"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "On"
		systemversion "10.0.19041.0"

		defines
		{
			"ENGINE_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "ENGINE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		optimize"On"

