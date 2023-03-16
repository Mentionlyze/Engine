#pragma once
#include "MaterialTexture.h"

namespace Engine
{
	class ModelTexture
	{
	public:
		
		virtual ~ModelTexture() = default;

		virtual void AddMaterialTexture(const std::string& path, const std::string& name) = 0;

		static Ref<ModelTexture> Create();
		static Ref<ModelTexture> Create(const std::initializer_list<MaterialTexture>& textures);
	};
}

