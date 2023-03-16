#include "Precompile.h"
#include "OpenGLMesh.h"
#include "OpenGLShader.h"
#include "glad/glad.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine
{
	OpenGLMesh::OpenGLMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<MaterialTexture>& textures)
		: m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
	{
		m_VertexArray = VertexArray::Create();
		m_VertexBuffer = VertexBuffer::Create(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));
		m_IndexBuffer = IndexBuffer::Create(&m_Indices[0], m_Indices.size());

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Pos" }, 
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord"}
		};

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

	OpenGLMesh::OpenGLMesh(const Ref<Geometry>& geometry, const std::vector<MaterialTexture>& textures) : m_Geometry(geometry) , m_Textures(textures)
	{

	}

	OpenGLMesh::~OpenGLMesh()
	{

	}

	void OpenGLMesh::Submit(const Ref<Shader>& shader, const glm::mat4& transform) const 
	{
		// bind appropriate textures
		uint32_t diffuseNr  = 1;
		uint32_t specularNr = 1;
		uint32_t normalNr   = 1;
		uint32_t heightNr   = 1;

		for (uint32_t i = 0; i < m_Textures.size(); i++)
		{
			std::string number;
			std::string name = m_Textures[i].Type;

			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);

			std::dynamic_pointer_cast<OpenGLShader>(shader)->SetInt((name + number), i);
			m_Textures[i].Texture->Bind(i);
		}

		Renderer::Submit(m_VertexArray, shader, transform);
		glActiveTexture(GL_TEXTURE0);
	}

	void OpenGLMesh::Submit(const Ref<Shader>& shader) const
	{
		for (uint32_t i = 0; i < m_Textures.size(); i++)
		{
			std::string name = m_Textures[i].Type;
			std::dynamic_pointer_cast<OpenGLShader>(shader)->SetInt(name, i);
			m_Textures[i].Texture->Bind(i);
		}

		Renderer::Submit(m_Geometry->GetVertextArray(), shader, m_Geometry->GetTransform());
		glActiveTexture(GL_TEXTURE0);
	}
}
