#include "Precompile.h"
#include "CubeMap.h"
#include "PlatForm/OpenGL/OpenGLCubeMap.h"

namespace Engine
{ 
	Ref<CubeMap> CubeMap::Create(const std::vector<std::string>& faces)
	{
		return CreateRef<OpenGLCubeMap>(faces);
	}
}
