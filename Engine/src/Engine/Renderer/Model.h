#pragma once
#include "Shader.h"
#include <glm/glm.hpp>

namespace Engine
{
	class Model
	{
	public:
		virtual ~Model() = default;
		
		virtual void Submit(const Ref<Shader>& shader, const glm::mat4& transform) const = 0;

		static Ref<Model> Create(const std::string& path, bool gamma = false);
	};
}


