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
IncludeDir["Glad"] = "Engine/vendor/Glad/include"
IncludeDir["ImGui"] = "Engine/vendor/imgui"
IncludeDir["glm"] = "Engine/vendor/glm"
IncludeDir["stb_image"] = "Engine/vendor/stb_image"
IncludeDir["assimp"] = "Engine/vendor/assimp/include"

include "Engine/vendor/GLFW"
include "Engine/vendor/Glad"
include "Engine/vendor/imgui"
include "Engine/vendor/assimp"


project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "Precompile.h"
	pchsource "Engine/src/Precompile.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.assimp}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"Assimp",
		"opengl32.lib",
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "off"
		systemversion "latest"

		defines
		{
			"ENGINE_PLATFORM_WINDOWS",
			"ENGINE_BUILD_DLL",
			"ENGINE_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "ENGINE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		runtime "Release"
		optimize"on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Engine/src",
		"Engine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links
	{
		"Engine"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "off"
		systemversion "latest"

		defines
		{
			"ENGINE_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "ENGINE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		runtime "Release"
		optimize"on"