#pragma once
#include "Shader.h"
#include "Vertex.h"
#include "MaterialTexture.h"
#include "Geometry.h"

namespace Engine
{
	class Mesh
	{
	public:
		virtual ~Mesh() = default;
		virtual void Submit(const Ref<Shader>& shader, const glm::mat4& transform, bool withCamera = true) const = 0;
		virtual void Submit(const Ref<Shader>& shader, bool withCamera = true) const = 0;

		static Ref<Mesh> Create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MaterialTexture>& textures);
		static Ref<Mesh> Create(const Ref<Geometry> geometry,  const std::vector<MaterialTexture>& textures);

	private:

	};
}

