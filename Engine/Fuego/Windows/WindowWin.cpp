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
        case WM_ACTIVATE:
            break;

        case WM_MOUSEMOVE:
        {
            float x = static_cast<float>(GET_X_LPARAM(lparam));
            float y = static_cast<float>(GET_Y_LPARAM(lparam));
            m_EventQueue->PushEvent(std::make_shared<MouseMovedEvent>(x, y));
            break;
        }

        case WM_LBUTTONDOWN:
            m_EventQueue->PushEvent(
                std::make_shared<MouseButtonPressedEvent>(Mouse::Button0));
            break;

        case WM_LBUTTONUP:
            m_EventQueue->PushEvent(
                std::make_shared<MouseButtonReleasedEvent>(Mouse::Button0));
            break;

        case WM_RBUTTONDOWN:
            m_EventQueue->PushEvent(
                std::make_shared<MouseButtonPressedEvent>(Mouse::Button1));
            break;

        case WM_RBUTTONUP:
            m_EventQueue->PushEvent(
                std::make_shared<MouseButtonReleasedEvent>(Mouse::Button1));
            break;

        case WM_MBUTTONDOWN:
            m_EventQueue->PushEvent(
                std::make_shared<MouseButtonPressedEvent>(Mouse::Button2));
            break;

        case WM_MBUTTONUP:
            m_EventQueue->PushEvent(
                std::make_shared<MouseButtonReleasedEvent>(Mouse::Button2));
            break;

        case WM_MOUSEWHEEL:
        {
            float yOffset = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam));
            m_EventQueue->PushEvent(
                std::make_shared<MouseScrolledEvent>(yOffset, 0.f));
            break;
        }

        case WM_MOUSEHWHEEL:
        {
            float xOffset = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam));
            m_EventQueue->PushEvent(
                std::make_shared<MouseScrolledEvent>(0.f, xOffset));
            break;
        }

        case WM_KEYDOWN:
        {
            int repeatCount = (lparam >> 16) & 0xFF;
            m_EventQueue->PushEvent(std::make_shared<KeyPressedEvent>(
                static_cast<int>(wparam), repeatCount));
            break;
        }
        case WM_KEYUP:
            m_EventQueue->PushEvent(
                std::make_shared<KeyReleasedEvent>(static_cast<int>(wparam)));
            break;

        case WM_CLOSE:
            m_EventQueue->PushEvent(std::make_shared<WindowCloseEvent>());
            break;

        case WM_SIZE:
        {
            UINT width = LOWORD(lparam);
            UINT height = HIWORD(lparam);

            m_EventQueue->PushEvent(
                std::make_shared<WindowResizeEvent>(width, height));
        }

        default:
            break;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowWin::WindowWin(const WindowProps& props, EventQueue& eventQueue)
    : m_EventQueue(dynamic_cast<EventQueueWin*>(
          &eventQueue)),  // do not transfer ownership
      m_Window(nullptr),
      m_HInstance(GetModuleHandle(nullptr)),
      m_Hwnd(nullptr),
      m_Props(props)
{
    static TCHAR buffer[32] = TEXT("");
#ifdef UNICODE
    MultiByteToWideChar(
        CP_UTF8, 0, props.Title.c_str(), -1, buffer, _countof(buffer));
#else
    sprintf_s(buffer, props.Title.c_str());
#endif
    WNDCLASSEX wndClass = {};
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.lpszClassName = m_Props.APP_WINDOW_CLASS_NAME;
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
