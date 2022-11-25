#include "Precompile.h"
#include "OrthoGraphicCamera.h"

#include <glm/gtx/transform.hpp>

namespace Engine
{
	OrthoGraphicsCamera::OrthoGraphicsCamera(float left, float right, float top, float bottom)
		: m_ProjectionMatrix(glm::ortho(left, right, top, bottom, -1.0f, 1.0f)), 
		  m_ViewMatrix(1.0f),
		  m_Position({ 0.0f, 0.0f, 0.0f }),
		  m_Rotation(0.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthoGraphicsCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
