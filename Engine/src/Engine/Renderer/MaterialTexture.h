#pragma once
#include "Texture.h"

namespace Engine
{
	struct MaterialTexture
	{
		unsigned int Id;
		std::string Type;
		std::string Path;
		Ref<Texture2D> Texture;
	};
}
