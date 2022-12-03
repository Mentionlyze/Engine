#pragma once

#include "Engine/Renderer/Camera.h"

namespace Engine
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float verticleFov, float aspectRatio, float nearClip, float farClip);

		const glm::vec3& GetForwardDirection() const { return m_ForwadDirection; }
		const glm::vec3& GetRightDirection() const { return m_RightDirection; }
		const glm::vec3& GetUpDirection() const { return m_UpDirection; }

		void UpdateRotation();

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
			RecalculateViewMatrix();
		}

		void setRotation(const glm::vec2 delta);

		void SetProjection(float verticleFov, float aspectRatio, float nearClip, float farClip);
	
		virtual const glm::mat4& GetProjectionMatrix() const override
		{
			return m_ProjectionMatrix;
		}

		virtual const glm::mat4& GetViewMatrix() const override
		{
			return m_ViewMatrix;
		}
		virtual const glm::mat4& GetViewProjectionMatrix() const override
		{
			return m_ViewProjectionMatrix;
		}

	private:
		void RecalculateViewMatrix();

	private:
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;

		glm::vec3 m_ForwadDirection;
		glm::vec3 m_RightDirection;
		glm::vec3 m_UpDirection;
		glm::vec3 m_WordUp{ 0, 1, 0 };

		glm::vec3 m_Position;


		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

	};

}
