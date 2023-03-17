#include "Precompile.h"
#include "OpenGLPlaneGeometry.h"

namespace Engine
{
	OpenGLPlaneGeometry::OpenGLPlaneGeometry(const glm::mat4& transform) : m_Transform(transform)
	{
		m_VertexArray = VertexArray::Create();

		float vertices[4 * 8] = {
			// positions            // normals         // texcoords
			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
			 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
		};

		m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Pos"},
			{ShaderDataType::Float3, "a_Normal"},
			{ShaderDataType::Float2, "a_TexCoord"},
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

	OpenGLPlaneGeometry::~OpenGLPlaneGeometry()
	{
	}

	void OpenGLPlaneGeometry::SetTransform(const glm::mat4& transform)
	{
		m_Transform = transform;
	}
}

