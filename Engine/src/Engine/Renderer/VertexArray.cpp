#include "Precompile.h"
#include "Engine/Renderer/VertexArray.h"
#include "PlatForm/OpenGL/OpenGLVertexArray.h"

namespace Engine
{
	VertexArray* VertexArray::Create()
	{
		return new OpenGLVertexArray();
	}
}