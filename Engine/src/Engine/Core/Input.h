#pragma once

#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"
#include <glm/glm.hpp>

namespace Engine
{
	class ENGINE_API Input 
	{
	public:
		inline static bool IsKeyPressed(const KeyCode keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static bool IsMouseButtonReleased(MouseCode button) { return s_Instance->IsMouseButtonReleasedImpl(button); }
		inline static glm::vec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(const KeyCode keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual bool IsMouseButtonReleasedImpl(MouseCode button) = 0;
		virtual glm::vec2 GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		
	private:
		static Input* s_Instance;
	};
}
