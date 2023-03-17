#include "Precompile.h"
#include "OpenGLBoxGeometry.h"

namespace Engine {

    OpenGLBoxGeometry::OpenGLBoxGeometry(const glm::mat4& transform) : m_Transform(transform)
    {
	    m_VertexArray = VertexArray::Create();

        float vertices[8 * 24] = {
            0.5, 0.5, 0.5, 1, 0, 0, 0, 1, 0.5, 0.5, -0.5, 1, 0, 0, 1, 1, 0.5, -0.5, 0.5, 1, 0, 0, 0, 0, 0.5, -0.5, -0.5, 1, 0, 0, 1, 0,
  -0.5, 0.5, -0.5, -1, 0, 0, 0, 1, -0.5, 0.5, 0.5, -1, 0, 0, 1, 1, -0.5, -0.5, -0.5, -1, 0, 0, 0, 0, -0.5, -0.5, 0.5, -1, 0, 0, 1,
  0, -0.5, 0.5, -0.5, 0, 1, 0, 0, 1, 0.5, 0.5, -0.5, 0, 1, 0, 1, 1, -0.5, 0.5, 0.5, 0, 1, 0, 0, 0, 0.5, 0.5, 0.5, 0, 1, 0, 1, 0,
  -0.5, -0.5, 0.5, 0, -1, 0, 0, 1, 0.5, -0.5, 0.5, 0, -1, 0, 1, 1, -0.5, -0.5, -0.5, 0, -1, 0, 0, 0, 0.5, -0.5, -0.5, 0, -1, 0, 1,
  0, -0.5, 0.5, 0.5, 0, 0, 1, 0, 1, 0.5, 0.5, 0.5, 0, 0, 1, 1, 1, -0.5, -0.5, 0.5, 0, 0, 1, 0, 0, 0.5, -0.5, 0.5, 0, 0, 1, 1, 0,
  0.5, 0.5, -0.5, 0, 0, -1, 0, 1, -0.5, 0.5, -0.5, 0, 0, -1, 1, 1, 0.5, -0.5, -0.5, 0, 0, -1, 0, 0, -0.5, -0.5, -0.5, 0, 0, -1, 1,
  0,
        };

       	m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Pos"},
			{ShaderDataType::Float3, "a_Normal"},
			{ShaderDataType::Float2, "a_TexCoord"},
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        uint32_t indices[6 * 6] = {
            0, 2, 1, 2, 3, 1, 4, 6, 5, 6, 7, 5, 8, 10, 9, 10, 11, 9, 12, 14, 13, 14, 15, 13, 16, 18, 17, 18, 19, 17, 20, 22, 21, 22, 23, 21,
        };

       	m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    }

	OpenGLBoxGeometry::~OpenGLBoxGeometry()
	{
	}

	void OpenGLBoxGeometry::SetTransform(const glm::mat4& transform) 
	{
		m_Transform = transform;
	}

}
