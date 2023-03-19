#include "Precompile.h"
#include "Buffer.h"
#include "Mesh.h"

#include "PlatForm/OpenGL/OpenGLBuffer.h"

namespace Engine
{
	Ref<VertexBuffer> VertexBuffer::Create(const void *vertices, uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(vertices, size);
	}

	Ref<IndexBuffer> IndexBuffer::Create(const void *indices, uint32_t count)
	{
		return CreateRef<OpenGLIndexBuffer>(indices, count);
	}

	Ref<FrameBuffer> FrameBuffer::Create()
	{
		return CreateRef<OpenGLFrameBuffer>();
	}
}
