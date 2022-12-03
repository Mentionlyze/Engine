#include "Precompile.h"
#include "OrthoGraphicCameraController.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Core.h"

namespace Engine
{

	OrthoGraphicCameraController::OrthoGraphicCameraController(float aspectRatio, bool rotateEnabled)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_RotateEnabled(rotateEnabled)
	{
	}

	void OrthoGraphicCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(Key::A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(Key::D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(Key::W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y +=  cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -=  cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (m_RotateEnabled)
		{
		if (Input::IsKeyPressed(Key::Q))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Input::IsKeyPressed(Key::E))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
	}

	void OrthoGraphicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(OrthoGraphicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(OrthoGraphicCameraController::OnWindowResized));
	}

	bool OrthoGraphicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthoGraphicCameraController::OnWindowResized(WindowResizeEvent& e)
	{

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}
