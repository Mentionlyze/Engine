#include "Precompile.h"
#include "Mesh.h"
#include "PlatForm/OpenGL/OpenGLMesh.h"

namespace Engine
{
	Ref<Mesh> Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<ModelTexture>& textures)
	{
		return CreateRef<OpenGLMesh>(vertices, indices, textures);
	}
	Ref<Mesh> Mesh::Create(const Ref<Geometry> geometry, const std::vector<ModelTexture>& textures)
	{
		return CreateRef<OpenGLMesh>(geometry, textures);
	}
}
