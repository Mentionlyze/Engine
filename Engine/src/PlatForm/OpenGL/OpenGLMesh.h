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
		std::vector<ModelTexture> m_Textures;

	public:
		OpenGLMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<ModelTexture>& textures);
		~OpenGLMesh();

		void Submit(const Ref<Shader>& shader, const glm::mat4& transform) const override;

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}
