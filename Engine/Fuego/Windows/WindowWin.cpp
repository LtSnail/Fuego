#include "WindowWin.h"

namespace Fuego
{
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

WindowWin::WindowWin(const WindowProps& props, EventQueue& eventQueue)
    : m_HInstance(GetModuleHandle(nullptr)), m_Window(nullptr), m_Hwnd(nullptr)
{
    UNUSED(eventQueue);
    m_Props = props;

    static TCHAR buffer[32] = TEXT("");
#ifdef UNICODE
    MultiByteToWideChar(
        CP_UTF8, 0, props.Title.c_str(), -1, buffer, _countof(buffer));
#else
    sprintf_s(buffer, props.Title.c_str());
#endif
    WNDCLASSEX wndClass = {};
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.lpszClassName = props.APP_WINDOW_CLASS_NAME;
    wndClass.hInstance = m_HInstance;
    wndClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.lpfnWndProc = WindowProc;

    RegisterClassEx(&wndClass);

    DWORD style = WS_OVERLAPPEDWINDOW | WS_SYSMENU;
    RECT rect;
    rect.left = props.x;
    rect.top = props.y;
    rect.right = rect.left + props.Width;
    rect.bottom = rect.top + props.Height;

    AdjustWindowRect(&rect, style, true);

    m_Hwnd = CreateWindowEx(0,
                            props.APP_WINDOW_CLASS_NAME,
                            buffer,
                            style,
                            rect.left,
                            rect.top,
                            rect.right - rect.left,
                            rect.bottom - rect.top,
                            nullptr,
                            nullptr,
                            m_HInstance,
                            nullptr);
    FU_CORE_ASSERT(m_Hwnd, "[AppWindow] hasn't been initialized!");
    ShowWindow(m_Hwnd, SW_SHOW);
}

void WindowWin::Update()
{
    MSG msg{};

    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void WindowWin::SetVSync(bool enabled) { UNUSED(enabled); }

bool WindowWin::IsVSync() const { return true; }

void WindowWin::Init(const WindowProps& props, EventQueue& eventQueue)
{
    UNUSED(props);
    UNUSED(eventQueue);
}

void WindowWin::Shutdown()
{
    if (m_Window != nullptr)
    {
        UnregisterClass(m_Props.APP_WINDOW_CLASS_NAME, m_HInstance);
    }
}

std::unique_ptr<Window> Window::CreateAppWindow(const WindowProps& props,
                                                EventQueue& eventQueue)
{
    return std::make_unique<WindowWin>(props, eventQueue);
}
}  // namespace Fuego
