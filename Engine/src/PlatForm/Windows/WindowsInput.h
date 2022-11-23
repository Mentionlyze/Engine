#pragma once

#include "Engine/Input.h"
#include "Engine/KeyCodes.h"
#include "Engine/MouseCodes.h"

namespace Engine
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(const KeyCode keycode) override;
		virtual bool IsMouseButtonPressedImpl(const MouseCode button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}

