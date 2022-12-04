#include "Precompile.h"
#include "Window.h"

#include "PlatForm/Windows/WindowsWindow.h"

namespace Engine
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
	}
}