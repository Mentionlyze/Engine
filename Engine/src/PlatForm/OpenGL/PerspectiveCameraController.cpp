#include "Precompile.h"
#include "Engine/Core/Input.h"
#include "PerspectiveCameraController.h"

namespace Engine
{
	PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float nearClip, float farClip)
		: m_VerticleFov(fov), 
		  m_AspectRatio(aspectRatio),
		  m_nearClip(nearClip),
		  m_farClip(farClip),
		  m_Camera(m_VerticleFov, m_AspectRatio, m_nearClip, m_farClip)
	{
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		float velocity = m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(Key::A))
			m_CameraPosition -= m_Camera.GetRightDirection() * velocity;
		else if (Input::IsKeyPressed(Key::D))
			m_CameraPosition += m_Camera.GetRightDirection() * velocity;
		else if (Input::IsKeyPressed(Key::W))
			m_CameraPosition += m_Camera.GetForwardDirection() * velocity;
		else if (Input::IsKeyPressed(Key::S))
			m_CameraPosition -= m_Camera.GetForwardDirection() * velocity;
		else if (Input::IsKeyPressed(Key::Q))
			m_CameraPosition += m_Camera.GetUpDirection() * velocity;
		else if (Input::IsKeyPressed(Key::E))
			m_CameraPosition -= m_Camera.GetUpDirection() * velocity;

		m_Camera.SetPosition(m_CameraPosition);

		if (Input::IsMouseButtonPressed(Mouse::Button1))
		{
			if (!m_CameraRotation)
				m_LastMousePosition = Input::GetMousePosition();

			m_CameraRotation = true;
		}
		else if (Input::IsMouseButtonReleased(Mouse::Button1))
			m_CameraRotation = false;

		if (m_CameraRotation) {
			glm::vec2 mousePos = Input::GetMousePosition();
			glm::vec2 delta;
			delta.x = (mousePos.x - m_LastMousePosition.x) * m_CameraRatationSpeed;
			delta.y = (m_LastMousePosition.y - mousePos.y) * m_CameraRatationSpeed;
			m_LastMousePosition = mousePos;
			m_Camera.setRotation(delta);
		}
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
		if (m_CameraRotation)
		{
			dispatcher.Dispatch<MouseMoveEvent>(ENGINE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseMoved));
		}
	}



	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_CameraPosition += (float)e.GetYOffset() * 0.25f * m_Camera.GetForwardDirection();
		m_Camera.SetPosition(m_CameraPosition);
		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(m_VerticleFov, m_AspectRatio, m_nearClip, m_farClip);
		return false;
	}

	bool PerspectiveCameraController::OnMouseMoved(MouseMoveEvent& e)
	{
		
		return false;
	}
}
