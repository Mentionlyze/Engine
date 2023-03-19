#pragma once
#include "Texture.h"

namespace Engine
{
	struct MaterialTexture
	{
		uint32_t Id;
		std::string Type;
		std::string Path;
		Ref<Texture> Texture;
	};
}
