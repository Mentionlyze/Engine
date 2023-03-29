#pragma once

#include "Engine/Core/Timestep.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "PerspectiveCamera.h"

namespace Engine
{
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float fov, float aspectRatio, float nearClip, float farClip);

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void SetPosition(const glm::vec3& position) { m_CameraPosition = position; };

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMouseMoved(MouseMoveEvent& e);

	private:
		float m_VerticleFov;
		float m_AspectRatio;
		float m_nearClip;
		float m_farClip;

		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;

		PerspectiveCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 6.0f };
		float m_CameraTranslationSpeed = 5.0f;

		bool m_CameraRotation;
		float m_CameraRatationSpeed = 0.1f;

		glm::vec2 m_LastMousePosition = glm::vec2(0.0f);
	};
}
