#include "WindowWin.h"

#include "InputWin.h"
#include "Log.h"
#include "OpenGL/SurfaceOpenGL.h"
#include "glad/glad.h"

namespace Fuego
{

DWORD WINAPI WindowWin::WinThreadMain(LPVOID lpParameter)
{
    WindowWin* _wnd = reinterpret_cast<WindowWin*>(lpParameter);

    static TCHAR buffer[32] = TEXT("");
#ifdef UNICODE
    MultiByteToWideChar(CP_UTF8, 0, props.Title.c_str(), -1, buffer, _countof(buffer));
#else
    sprintf_s(buffer, _wnd->m_Props.Title.c_str());
#endif
    WNDCLASSEX wndClass = {};
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.lpszClassName = _wnd->m_Props.APP_WINDOW_CLASS_NAME;
    wndClass.hInstance = _wnd->m_HInstance;
    wndClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.lpfnWndProc = WindowProcStatic;

    RegisterClassEx(&wndClass);

    DWORD style = WS_OVERLAPPEDWINDOW | WS_SYSMENU;
    RECT rect;
    rect.left = _wnd->m_Props.x;
    rect.top = _wnd->m_Props.y;
    rect.right = rect.left + _wnd->m_Props.Width;
    rect.bottom = rect.top + _wnd->m_Props.Height;

    AdjustWindowRect(&rect, style, true);

    _wnd->_surface = new Renderer::SurfaceOpenGL(CreateWindowEx(0, _wnd->m_Props.APP_WINDOW_CLASS_NAME, buffer, style, rect.left, rect.top,
                                                                rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, _wnd->m_HInstance, nullptr));

    FU_CORE_ASSERT(_wnd->_surface, "[AppWindow] hasn't been initialized!");
    hwndMap.emplace(_wnd->_surface->GetWindowsHandle(), _wnd);

    ShowWindow(_wnd->_surface->GetWindowsHandle(), SW_SHOW);

    FU_CORE_ASSERT(Input::Init(new InputWin()), "[Input] hasn't been initialized!");
    SetEvent(_wnd->_onThreadCreated);

    MSG msg{};
    while (GetMessage(&msg, nullptr, 0u, 0u))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    // Cleanup
    ReleaseDC(_wnd->_surface->GetWindowsHandle(), _wnd->_surface->GetHDC());
    DestroyWindow(_wnd->_surface->GetWindowsHandle());
    DestroyIcon(wndClass.hIcon);
    DestroyCursor(wndClass.hCursor);
    UnregisterClass(_wnd->m_Props.APP_WINDOW_CLASS_NAME, _wnd->m_HInstance);

    return S_OK;
}

std::unordered_map<HWND, WindowWin*> WindowWin::hwndMap;

LRESULT CALLBACK WindowWin::WindowProcStatic(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
    case WM_PAINT:
        _eventQueue->PushEvent(std::make_shared<EventVariant>(AppRenderEvent()));
        return 0;
    case WM_ACTIVATE:
        break;

    case WM_MOUSEMOVE:
    {
        float x = static_cast<float>(GET_X_LPARAM(lparam));
        float y = static_cast<float>(GET_Y_LPARAM(lparam));
        _cursorPos = {x, y};
        _eventQueue->PushEvent(std::make_shared<EventVariant>(MouseMovedEvent(x, y)));
        break;
    }

    case WM_LBUTTONDOWN:
        _eventQueue->PushEvent(std::make_shared<EventVariant>(MouseButtonPressedEvent(Mouse::Button0)));
        break;

    case WM_LBUTTONUP:
        _eventQueue->PushEvent(std::make_shared<EventVariant>(MouseButtonReleasedEvent(Mouse::Button0)));
        break;

    case WM_RBUTTONDOWN:
        _eventQueue->PushEvent(std::make_shared<EventVariant>(MouseButtonPressedEvent(Mouse::Button1)));
        break;

    case WM_RBUTTONUP:
        _eventQueue->PushEvent(std::make_shared<EventVariant>(MouseButtonReleasedEvent(Mouse::Button1)));
        break;

    case WM_MBUTTONDOWN:
        _eventQueue->PushEvent(std::make_shared<EventVariant>(MouseButtonPressedEvent(Mouse::Button2)));
        break;

    case WM_MBUTTONUP:
        _eventQueue->PushEvent(std::make_shared<EventVariant>(MouseButtonReleasedEvent(Mouse::Button2)));
        break;

    case WM_MOUSEWHEEL:
    {
        float yOffset = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam));
        _eventQueue->PushEvent(std::make_shared<EventVariant>(MouseScrolledEvent(yOffset, 0.f)));
        break;
    }

    case WM_MOUSEHWHEEL:
    {
        float xOffset = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam));
        _eventQueue->PushEvent(std::make_shared<EventVariant>(MouseScrolledEvent(0.f, xOffset)));
        break;
    }

    case WM_KEYDOWN:
    {
        int repeatCount = (lparam >> 16) & 0xFF;
        int keyCode = static_cast<int>(wparam);
        _eventQueue->PushEvent(std::make_shared<EventVariant>(KeyPressedEvent(keyCode, repeatCount)));
        _lastKey = {Input::KEY_PRESSED, (uint16_t)keyCode};
        break;
    }
    case WM_KEYUP:
        _eventQueue->PushEvent(std::make_shared<EventVariant>(KeyReleasedEvent(static_cast<int>(wparam))));
        break;

    case WM_CLOSE:
        _eventQueue->PushEvent(std::make_shared<EventVariant>(WindowCloseEvent()));
        break;

    case WM_SIZE:
    {
        UINT width = LOWORD(lparam);
        UINT height = HIWORD(lparam);

        _eventQueue->PushEvent(std::make_shared<EventVariant>(WindowResizeEvent(width, height)));
        break;
    }

    default:
    {
        _lastKey = {Input::KEY_NONE, LAST_CODE};
        _lastMouse = {Input::MOUSE_NONE, LAST_CODE};
        break;
    }
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowWin::WindowWin(const WindowProps& props, EventQueue& eventQueue)
    : _eventQueue(dynamic_cast<EventQueueWin*>(&eventQueue))
    ,  // do not transfer ownership
    m_Window(nullptr)
    , m_HInstance(GetModuleHandle(nullptr))
    , m_Props(props)
    , _surface(nullptr)
    , _lastKey{Input::KEY_NONE, LAST_CODE}
    , _lastMouse{Input::MOUSE_NONE, LAST_CODE}
    , _cursorPos{0.f, 0.f}
    , _winThread{}
    , _winThreadID(nullptr)
    , _onThreadCreated(CreateEvent(nullptr, FALSE, FALSE, nullptr))
{
    _winThread = CreateThread(nullptr, 0, WinThreadMain, this, 0, _winThreadID);
    WaitForSingleObject(_onThreadCreated, INFINITE);
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

const Renderer::Surface* WindowWin::GetSurface() const
{
    return reinterpret_cast<const Renderer::Surface*>(_surface);
}

Input::KeyState WindowWin::GetKeyState(KeyCode keyCode) const
{
    return _lastKey.keyCode == keyCode ? _lastKey.state : Input::KeyState::KEY_NONE;
}

Input::MouseState WindowWin::GetMouseState(MouseCode mouseCode) const
{
    return _lastMouse.mouseCode == mouseCode ? _lastMouse.state : Input::MouseState::MOUSE_NONE;
}

void WindowWin::GetCursorPos(OUT float& xPos, OUT float& yPos) const
{
    xPos = _cursorPos.x;
    yPos = _cursorPos.y;
}

void WindowWin::Shutdown()
{
    if (_surface->GetNativeHandle() != nullptr)
    {
        DestroyWindow(_surface->GetWindowsHandle());
        delete _surface;
        _surface = nullptr;
    }
}

std::unique_ptr<Window> Window::CreateAppWindow(const WindowProps& props, EventQueue& eventQueue)
{
    return std::make_unique<WindowWin>(props, eventQueue);
}
}  // namespace Fuego
