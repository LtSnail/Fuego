#include "WindowWin.h"

namespace Fuego
{
	WindowWin::WindowWin(const WindowProps& props, EventQueue& eventQueue)
	{
		UNUSED(props);
		UNUSED(eventQueue);
	}

	void WindowWin::Update()
	{

	}

	void WindowWin::SetVSync(bool enabled)
	{
		UNUSED(enabled);
	}

	bool WindowWin::IsVSync() const
	{
		return true;
	}

	void WindowWin::Init(const WindowProps& props, EventQueue& eventQueue)
	{
		UNUSED(props);
		UNUSED(eventQueue);
	}

	void WindowWin::Shutdown()
	{

	}

	std::unique_ptr<Window> Window::CreateAppWindow(const WindowProps& props,
													EventQueue& eventQueue)
	{
		return std::make_unique<WindowWin>(props, eventQueue);
	}
}
