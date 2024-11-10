#include "WindowWin.h"

namespace Fuego
{
std::unordered_map<HWND, WindowWin*> WindowWin::hwndMap;

LRESULT CALLBACK WindowWin::WindowProcStatic(HWND hWnd, UINT uMsg,
                                             WPARAM wParam, LPARAM lParam)
{
    auto _this = WindowWin::hwndMap.find(hWnd);

    if (_this != WindowWin::hwndMap.end() && _this->second)
    {
        return _this->second->WindowProc(hWnd, uMsg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

LRESULT WindowWin::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_KEYDOWN:
        {
            int repeatCount = (lparam >> 16) & 0xFF;
            m_EventQueue.PushEvent(std::make_unique<KeyPressedEvent>(
                static_cast<int>(wparam), repeatCount));
            break;
        }
        case WM_KEYUP:
            m_EventQueue.PushEvent(
                std::make_unique<KeyReleasedEvent>(static_cast<int>(wparam)));
            break;

        case WM_CLOSE:
            m_EventQueue.PushEvent(std::make_unique<WindowCloseEvent>());
            break;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowWin::WindowWin(const WindowProps& props, EventQueue& eventQueue)
    : m_HInstance(GetModuleHandle(nullptr)),
      m_Window(nullptr),
      m_Hwnd(nullptr),
      m_EventQueue(eventQueue)

{
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
    wndClass.lpfnWndProc = WindowProcStatic;

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
    hwndMap.emplace(m_Hwnd, this);
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
    if (m_Hwnd != nullptr)
    {
        DestroyWindow(m_Hwnd);
        m_Hwnd = nullptr;
    }
}

std::unique_ptr<Window> Window::CreateAppWindow(const WindowProps& props,
                                                EventQueue& eventQueue)
{
    return std::make_unique<WindowWin>(props, eventQueue);
}
}  // namespace Fuego
