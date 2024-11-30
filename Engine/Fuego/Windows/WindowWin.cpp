#include "WindowWin.h"

#include "InputWin.h"
#include "Log.h"

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

    _wnd->m_Hwnd = CreateWindowEx(0, _wnd->m_Props.APP_WINDOW_CLASS_NAME, buffer, style, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                                  nullptr, nullptr, _wnd->m_HInstance, nullptr);
    FU_CORE_ASSERT(_wnd->m_Hwnd, "[AppWindow] hasn't been initialized!");
    hwndMap.emplace(_wnd->m_Hwnd, _wnd);

    

    _wnd->_hdc = GetDC(_wnd->m_Hwnd);
    ShowWindow(_wnd->m_Hwnd, SW_SHOW);
    FU_CORE_ASSERT(Input::Init(new InputWin()), "[Input] hasn't been initialized!");

    SetEvent(_wnd->_isCreatedEvent);

    MSG msg{};
    while (GetMessage(&msg, nullptr, 0u, 0u))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    // Cleanup
    ReleaseDC(_wnd->m_Hwnd, _wnd->_hdc);
    DestroyWindow(_wnd->m_Hwnd);
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
    {
        static PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
       
        return 0;
    }
    case WM_ACTIVATE:
        break;

    case WM_MOUSEMOVE:
    {
        float x = static_cast<float>(GET_X_LPARAM(lparam));
        float y = static_cast<float>(GET_Y_LPARAM(lparam));
        _cursorPos = {x, y};
        m_EventQueue->PushEvent(std::make_shared<MouseMovedEvent>(x, y));
        break;
    }

    case WM_LBUTTONDOWN:
        m_EventQueue->PushEvent(std::make_shared<MouseButtonPressedEvent>(Mouse::Button0));
        break;

    case WM_LBUTTONUP:
        m_EventQueue->PushEvent(std::make_shared<MouseButtonReleasedEvent>(Mouse::Button0));
        break;

    case WM_RBUTTONDOWN:
        m_EventQueue->PushEvent(std::make_shared<MouseButtonPressedEvent>(Mouse::Button1));
        break;

    case WM_RBUTTONUP:
        m_EventQueue->PushEvent(std::make_shared<MouseButtonReleasedEvent>(Mouse::Button1));
        break;

    case WM_MBUTTONDOWN:
        m_EventQueue->PushEvent(std::make_shared<MouseButtonPressedEvent>(Mouse::Button2));
        break;

    case WM_MBUTTONUP:
        m_EventQueue->PushEvent(std::make_shared<MouseButtonReleasedEvent>(Mouse::Button2));
        break;

    case WM_MOUSEWHEEL:
    {
        float yOffset = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam));
        m_EventQueue->PushEvent(std::make_shared<MouseScrolledEvent>(yOffset, 0.f));
        break;
    }

    case WM_MOUSEHWHEEL:
    {
        float xOffset = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam));
        m_EventQueue->PushEvent(std::make_shared<MouseScrolledEvent>(0.f, xOffset));
        break;
    }

    case WM_KEYDOWN:
    {
        int repeatCount = (lparam >> 16) & 0xFF;
        int keyCode = static_cast<int>(wparam);
        m_EventQueue->PushEvent(std::make_shared<KeyPressedEvent>(keyCode, repeatCount));
        _lastKey = {Input::KEY_PRESSED, (uint16_t)keyCode};
        break;
    }
    case WM_KEYUP:
        m_EventQueue->PushEvent(std::make_shared<KeyReleasedEvent>(static_cast<int>(wparam)));
        break;

    case WM_CLOSE:
        m_EventQueue->PushEvent(std::make_shared<WindowCloseEvent>());
        break;

    case WM_SIZE:
    {
        UINT width = LOWORD(lparam);
        UINT height = HIWORD(lparam);

        m_EventQueue->PushEvent(std::make_shared<WindowResizeEvent>(width, height));
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
    : m_EventQueue(dynamic_cast<EventQueueWin*>(&eventQueue))
    ,  // do not transfer ownership
    m_Window(nullptr)
    , m_HInstance(GetModuleHandle(nullptr))
    , m_Hwnd(nullptr)
    , m_Props(props)
    , _lastKey{Input::KEY_NONE, LAST_CODE}
    , _lastMouse{Input::MOUSE_NONE, LAST_CODE}
    , _cursorPos{0.f, 0.f}
    , _winThread{}
    , _winThreadID(nullptr)
    , _context(nullptr)
    , _hdc(nullptr)
    , _isCreatedEvent(nullptr)
{
    _winThread = CreateThread(nullptr, 0, WinThreadMain, this, 0, _winThreadID);
    _isCreatedEvent = CreateEvent(nullptr, false, false, "Second Thread creation");
    WaitForSingleObject(_isCreatedEvent, INFINITE);
    _context = new OpenGLContext(&m_Hwnd);
    FU_CORE_ASSERT(_context->Init(), "[GraphicsContext] hasn't been initialized!");
}

void WindowWin::Update()
{
    glClearColor(0.2f, 0.3f, 0.1f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    SwapBuffers(_hdc);
}

void WindowWin::SetVSync(bool enabled)
{
    UNUSED(enabled);
}

bool WindowWin::IsVSync() const
{
    return true;
};

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
    if (m_Hwnd != nullptr)
    {
        DestroyWindow(m_Hwnd);
        m_Hwnd = nullptr;
    }
}

std::unique_ptr<Window> Window::CreateAppWindow(const WindowProps& props, EventQueue& eventQueue)
{
    return std::make_unique<WindowWin>(props, eventQueue);
}
}  // namespace Fuego
