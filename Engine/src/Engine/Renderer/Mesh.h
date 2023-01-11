#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

namespace Engine
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct ModelTexture
	{
		unsigned int Id;
		std::string Type;
		std::string Path;
	};

	class Mesh
	{
	public:
		virtual ~Mesh() = default;
		virtual void Submit(const Ref<Shader>& shader, const glm::mat4& transform) const = 0;

		static Ref<Mesh> Create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<ModelTexture>& textures);

	private:

	};
}

