#include "Precompile.h"
#include "PerspectiveCamera.h"
#include <glm/gtx/transform.hpp>

namespace Engine
{
	PerspectiveCamera::PerspectiveCamera(float verticleFov, float aspectRatio, float nearClip, float farClip)
		: m_ProjectionMatrix(glm::perspective(glm::radians(verticleFov), aspectRatio, nearClip, farClip))
	{

		m_ForwadDirection = glm::vec3(0, 0, -1);
		m_Position = glm::vec3(0, 0, 6);
		UpdateRotation();
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::UpdateRotation() 
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_ForwadDirection = glm::normalize(front);
		// also re-calculate the Right and Up vector
		m_RightDirection = glm::normalize(glm::cross(m_ForwadDirection, m_WordUp));
		// normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_UpDirection = glm::normalize(glm::cross(m_RightDirection, m_ForwadDirection));

	}

	void PerspectiveCamera::setRotation(const glm::vec2 delta)
	{
		m_Yaw += delta.x;
		m_Pitch += delta.y;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		UpdateRotation();
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetProjection(float verticleFov, float aspectRatio, float nearClip, float farClip)
	{
		m_ProjectionMatrix = glm::perspective(verticleFov, aspectRatio, nearClip, farClip);
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_ForwadDirection, m_UpDirection);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
