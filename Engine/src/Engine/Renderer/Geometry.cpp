#include "Precompile.h"
#include "Geometry.h"
#include "PlatForm/OpenGL/OpenGLBoxGeometry.h"
#include "PlatForm/OpenGL/OpenGLPlaneGeometry.h"

namespace Engine {

	Ref<Geometry> Geometry::CreateBox(const glm::mat4& transform)
	{
		return CreateRef<OpenGLBoxGeometry>(transform);
	}

	Ref<Geometry> Geometry::CreatePlane(const glm::mat4& transform)
	{
		return CreateRef<OpenGLPlaneGeometry>(transform);
	}
}

