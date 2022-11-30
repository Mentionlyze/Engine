#include "Precompile.h"
#include "OrthoGraphicCameraController.h"

namespace Engine
{

	OrthoGraphicCameraController::OrthoGraphicCameraController(float aspectRatio, bool rotateEnabled)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_RotateEnabled(rotateEnabled)
	{
	}

	void OrthoGraphicCameraController::OnUpdate(Timestep ts)
	{
	}

	void OrthoGraphicCameraController::OnEvent(Event& e)
	{
	}

	bool OrthoGraphicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		return false;
	}

	bool OrthoGraphicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		return false;
	}

}
