#include "Precompile.h"
#include "Engine/Renderer/VertexArray.h"
#include "PlatForm/OpenGL/OpenGLVertexArray.h"

namespace Engine
{
	Ref<VertexArray> VertexArray::Create()
	{
		return CreateRef<OpenGLVertexArray>();
	}
}