#include "WinWindow.h"

namespace Fuego
{
	WindowsWindow::WindowsWindow(const WindowProps& props, EventQueue& eventQueue)
	{
		UNUSED(props);
		UNUSED(eventQueue);
	}

	WindowsWindow::~WindowsWindow()
	{

	}

	void WindowsWindow::Update()
	{

	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		UNUSED(enabled);
	}

	bool WindowsWindow::IsVSync() const
	{
		return true;
	}

	void WindowsWindow::Init(const WindowProps& props, EventQueue& eventQueue)
	{
		UNUSED(props);
		UNUSED(eventQueue);
	}

	void WindowsWindow::Shutdown()
	{

	}

	std::unique_ptr<Window> Window::CreateAppWindow(const WindowProps& props,
													EventQueue& eventQueue)
	{
		return std::make_unique<WindowsWindow>(props, eventQueue);
	}
}
