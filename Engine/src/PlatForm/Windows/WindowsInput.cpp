#include "Precompile.h"
#include "WindowsInput.h"

#include "Engine/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Engine
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(const KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}


	bool WindowsInput::IsMouseButtonPressedImpl(const MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	bool WindowsInput::IsMouseButtonReleasedImpl(MouseCode button)
	{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_RELEASE;
	}
	
	glm::vec2 WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xpos, ypos;

		glfwGetCursorPos(window, &xpos, &ypos);

		return glm::vec2((float)xpos, (float)ypos);
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto mousePos = GetMousePositionImpl();
		return mousePos.x;
	}
	
	float WindowsInput::GetMouseYImpl()
	{
		auto mousePos = GetMousePositionImpl();
		return mousePos.y;
	}
}
