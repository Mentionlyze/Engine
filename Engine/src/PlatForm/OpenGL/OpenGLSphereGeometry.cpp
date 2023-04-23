#include "Precompile.h"
#include "OpenGLSphereGeometry.h"

namespace Engine
{
	OpenGLSphereGeometry::OpenGLSphereGeometry(const glm::mat4& transform) : m_Transform(transform)
	{
		m_VertexArray = Engine::VertexArray::Create();

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uv;

		std::vector<uint32_t> indices;

		const uint32_t X_SEGMENTS = 64;
		const uint32_t Y_SEGMENTS = 64;

		const float PI = 3.14159265359f;
		
		for (uint32_t x = 0; x <= X_SEGMENTS; ++x)
		{
			for (uint32_t y = 0; y <= Y_SEGMENTS; ++y) {
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;

				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (uint32_t y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) 
			{
				for (uint32_t x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}

			oddRow = !oddRow;
		}

		uint32_t indexCount = indices.size();
		
		std::vector<float> vertices;
		for (uint32_t i = 0; i < positions.size(); ++i)
		{
			vertices.push_back(positions[i].x);
			vertices.push_back(positions[i].y);
			vertices.push_back(positions[i].z);

			if (normals.size() > 0)
			{
				vertices.push_back(normals[i].x);
				vertices.push_back(normals[i].y);
				vertices.push_back(normals[i].z);
			}

			if (uv.size() > 0)
			{
				vertices.push_back(uv[i].x);
				vertices.push_back(uv[i].y);
			}
		}

		m_VertexBuffer = Engine::VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float));
		Engine::BufferLayout layout = {
			{ Engine::ShaderDataType::Float3, "a_Pos"},
			{ Engine::ShaderDataType::Float3, "a_Normal"},
			{ Engine::ShaderDataType::Float2, "a_TexCoords"},
		};
		m_VertexBuffer->SetLayout(layout);

		m_IndexBuffer = Engine::IndexBuffer::Create(&indices[0], indexCount);

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

	OpenGLSphereGeometry::~OpenGLSphereGeometry()
	{
	}

	void OpenGLSphereGeometry::SetTransform(const glm::mat4& transform)
	{
		m_Transform = transform;
	}
}

