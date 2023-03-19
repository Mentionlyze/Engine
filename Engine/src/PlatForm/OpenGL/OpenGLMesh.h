#pragma once
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Buffer.h"

namespace Engine
{
	class OpenGLMesh : public Mesh
	{
	public:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<MaterialTexture> m_Textures;

	public:
		OpenGLMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<MaterialTexture>& textures);
		OpenGLMesh(const Ref<Geometry>& geometry, const std::vector<MaterialTexture>& textures);
		~OpenGLMesh();

		Ref<VertexArray> GetVertexArray() { return m_VertexArray; };

		void Submit(const Ref<Shader>& shader, const glm::mat4& transform, bool withCamera) const override;
		void Submit(const Ref<Shader>& shader, bool withCamera) const override;

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		Ref<Geometry> m_Geometry;
	};
}
