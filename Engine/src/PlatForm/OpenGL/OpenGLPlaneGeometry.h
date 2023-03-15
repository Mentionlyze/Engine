#pragma once
#include "Engine/Renderer/Geometry.h"

namespace Engine
{
	class OpenGLPlaneGeometry : public Geometry
	{
	public:
		OpenGLPlaneGeometry(const glm::mat4& transform);
		virtual ~OpenGLPlaneGeometry();

		virtual Ref<VertexArray> GetVertextArray() const override { return m_VertexArray; };
		virtual glm::mat4 GetTransform() const override { return m_Transform; };

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		glm::mat4 m_Transform;
	};
}
