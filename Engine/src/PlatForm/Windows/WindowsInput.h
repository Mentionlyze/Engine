#pragma once

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"

namespace Engine
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(const KeyCode keycode) override;
		virtual bool IsMouseButtonPressedImpl(const MouseCode button) override;
		virtual bool IsMouseButtonReleasedImpl(MouseCode button) override;
		virtual glm::vec2 GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}

