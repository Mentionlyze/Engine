#pragma once
#include "VertexArray.h"
#include <glm/glm.hpp>

namespace Engine
{

	class Geometry
	{
	public:
		virtual ~Geometry() = default;

		virtual Ref<VertexArray> GetVertextArray() const = 0;
		virtual glm::mat4 GetTransform() const = 0;
		
		static Ref<Geometry> CreateBox(const glm::mat4& transform);
		static Ref<Geometry> CreatePlane(const glm::mat4& transform);
	};
}
