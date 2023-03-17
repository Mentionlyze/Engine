#pragma once
#include "Engine/Renderer/Geometry.h"

namespace Engine
{
	class OpenGLBoxGeometry : public Geometry {
	public:
		OpenGLBoxGeometry(const glm::mat4& transform);
		virtual ~OpenGLBoxGeometry();

		Ref<VertexArray> GetVertextArray() const override { return m_VertexArray; };
		void SetTransform(const glm::mat4& transform) override;
		glm::mat4 GetTransform() const override { return m_Transform; };

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		glm::mat4 m_Transform;

	};
}

