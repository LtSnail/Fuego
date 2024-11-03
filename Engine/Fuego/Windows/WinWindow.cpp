#include "WinWindow.h"

namespace Fuego
{
	WinWindow::WinWindow(const WindowProps& props, EventQueue& eventQueue)
	{
		UNUSED(props);
		UNUSED(eventQueue);
	}

	void WinWindow::Update()
	{

	}

	void WinWindow::SetVSync(bool enabled)
	{
		UNUSED(enabled);
	}

	bool WinWindow::IsVSync() const
	{
		return true;
	}

	void WinWindow::Init(const WindowProps& props, EventQueue& eventQueue)
	{
		UNUSED(props);
		UNUSED(eventQueue);
	}

	void WinWindow::Shutdown()
	{

	}

	std::unique_ptr<Window> Window::CreateAppWindow(const WindowProps& props,
													EventQueue& eventQueue)
	{
		return std::make_unique<WinWindow>(props, eventQueue);
	}
}
