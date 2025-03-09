#include "WindowWin.h"

#include <glad/gl.h>
#include <glad/wgl.h>

#include "InputWin.h"
#include "KeyCodesWin.h"
#include "Log.h"


typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int* attribList);
wglCreateContextAttribsARB_type* wglCreateContextAttribsARB;

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats,
                                                 UINT* nNumFormats);
wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;

namespace Fuego
{

DWORD WINAPI WindowWin::WinThreadMain(LPVOID lpParameter)
{
    InitOpenGLExtensions();

    WindowWin* window = static_cast<WindowWin*>(lpParameter);

    static TCHAR buffer[32] = TEXT("");
#ifdef UNICODE
    MultiByteToWideChar(CP_UTF8, 0, props.Title.c_str(), -1, buffer, _countof(buffer));
#else
    FU_CORE_ASSERT(sprintf_s(buffer, window->_props.Title.c_str()), "")
#endif
    WNDCLASSEX wndClass = {};
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.lpszClassName = window->_props.APP_WINDOW_CLASS_NAME;
    wndClass.hInstance = window->_hinstance;
    wndClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.lpfnWndProc = WindowProcStatic;

    RegisterClassEx(&wndClass);

    DWORD style = WS_OVERLAPPEDWINDOW | WS_SYSMENU;
    RECT rect;
    rect.left = window->_props.x;
    rect.top = window->_props.y;
    rect.right = rect.left + window->_props.Width;
    rect.bottom = rect.top + window->_props.Height;

    AdjustWindowRect(&rect, style, true);

    window->_hwnd = CreateWindowEx(0, window->_props.APP_WINDOW_CLASS_NAME, buffer, style, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                                   nullptr, nullptr, window->_hinstance, nullptr);

    FU_CORE_ASSERT(window->_hwnd, "[AppWindow] hasn't been initialized!");

    // Associate this instance with the HWND
    SetWindowLongPtr(window->_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));

    ShowWindow(window->_hwnd, SW_SHOW);

    FU_CORE_ASSERT(Input::Init(new InputWin()), "[Input] hasn't been initialized!");
    SetEvent(window->_onThreadCreated);

    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DestroyWindow(window->_hwnd);
    DestroyIcon(wndClass.hIcon);
    DestroyCursor(wndClass.hCursor);
    UnregisterClass(window->_props.APP_WINDOW_CLASS_NAME, window->_hinstance);

    return S_OK;
}

LRESULT CALLBACK WindowWin::WindowProcStatic(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (WindowWin* window = reinterpret_cast<WindowWin*>(GetWindowLongPtr(hWnd, GWLP_USERDATA)))
    {
        return window->WindowProc(hWnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void WindowWin::InitOpenGLExtensions()
{
    WNDCLASSA window_class = {
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = DefWindowProcA,
        .hInstance = GetModuleHandle(nullptr),
        .lpszClassName = "Dummy_Window",
    };

    if (!RegisterClassA(&window_class))
        FU_CORE_ERROR("Failed to register dummy OpenGL window");

    HWND dummy_window = CreateWindowExA(0, window_class.lpszClassName, "Dummy OpenGL Window", 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0,
                                        0, window_class.hInstance, nullptr);
    if (!dummy_window)
        FU_CORE_ERROR("Failed to create dummy OpenGL window.");

    HDC dummy_dc = GetDC(dummy_window);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    const int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
    if (!pixel_format)
    {
        FU_CORE_ERROR("Failed to find a suitable pixel format.");
    }
    if (!SetPixelFormat(dummy_dc, pixel_format, &pfd))
    {
        FU_CORE_ERROR("Failed to set the pixel format.");
    }

    const HGLRC dummy_context = wglCreateContext(dummy_dc);
    if (!dummy_context)
    {
        FU_CORE_ERROR("Failed to create a dummy OpenGL rendering context.");
    }

    if (!wglMakeCurrent(dummy_dc, dummy_context))
    {
        FU_CORE_ERROR("Failed to activate dummy OpenGL rendering context.");
    }

    wglCreateContextAttribsARB = reinterpret_cast<wglCreateContextAttribsARB_type*>(wglGetProcAddress("wglCreateContextAttribsARB"));
    wglChoosePixelFormatARB = reinterpret_cast<wglChoosePixelFormatARB_type*>(wglGetProcAddress("wglChoosePixelFormatARB"));

    wglMakeCurrent(dummy_dc, 0);
    wglDeleteContext(dummy_context);
    ReleaseDC(dummy_window, dummy_dc);
    DestroyWindow(dummy_window);
}

LRESULT WindowWin::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    is_in_focus = GetForegroundWindow() == hwnd;

    switch (msg)
    {
    case WM_ACTIVATE:
        break;
    case WM_PAINT:
    {
        if (!isPainted || isResizing || _props.mode == MINIMIZED)
            return 0;
        _eventQueue->PushEvent(std::make_shared<EventVariant>(WindowValidateEvent()));
        isPainted = false;
        return 0;
    }
    case WM_ENTERSIZEMOVE:
    {
        isResizing = true;
        _eventQueue->PushEvent(std::make_shared<EventVariant>(WindowStartResizeEvent()));
        break;
    }
    case WM_EXITSIZEMOVE:
    {
        isResizing = false;
        _eventQueue->PushEvent(std::make_shared<EventVariant>(WindowEndResizeEvent()));

        POINT center;
        center.x = _currentWidth / 2;
        center.y = _currentHeigth / 2;

        ClientToScreen(hwnd, &center);
        window_center_x = center.x;
        window_center_y = center.y;
        RECT rect;
        GetClientRect(hwnd, &rect);

        _xPos = rect.left;
        _yPos = rect.top;
        _eventQueue->PushEvent(std::make_shared<EventVariant>(WindowResizeEvent(_xPos, _yPos, _currentWidth, _currentHeigth)));
        break;
    }
    case WM_SIZE:
    {
        if (isResizing)
            break;

        _currentWidth = LOWORD(lparam);
        _currentHeigth = HIWORD(lparam);

        RECT r;
        GetWindowRect(hwnd, &r);
        _xPos = r.left;
        _yPos = r.top;
        _eventQueue->PushEvent(std::make_shared<EventVariant>(WindowResizeEvent(_xPos, _yPos, _currentWidth, _currentHeigth)));

        SetWindowMode(wparam);
        break;
    }
    case WM_MOUSEMOVE:
    {
        if (is_first_launch)
        {
            RECT rect;
            GetClientRect(hwnd, &rect);
            POINT center;
            center.x = _currentWidth / 2;
            center.y = _currentHeigth / 2;
            ClientToScreen(hwnd, &center);
            window_center_x = center.x;
            window_center_y = center.y;

            _cursorPos.x = window_center_x;
            _cursorPos.y = window_center_y;

            _prevCursorPos = _cursorPos;
            is_first_launch = false;
            SetCursorPos(window_center_x, window_center_y);
        }

        POINT cursor_pos;
        GetCursorPos(&cursor_pos);

        RECT client_rect;
        GetClientRect(hwnd, &client_rect);
        bool insideWindow =
            cursor_pos.x >= client_rect.left && cursor_pos.x <= client_rect.right && cursor_pos.y >= client_rect.top && cursor_pos.y <= client_rect.bottom;

        if (insideWindow && is_in_focus)
        {
            if (interaction_mode == InteractionMode::GAMING)
            {
                _prevCursorPos.x = window_center_x;
                _prevCursorPos.y = window_center_y;

                _cursorPos.x = cursor_pos.x;
                _cursorPos.y = cursor_pos.y;

                _mouseDir.x = _cursorPos.x - _prevCursorPos.x;
                _mouseDir.y = _cursorPos.y - _prevCursorPos.y;

                SetCursorPos(window_center_x, window_center_y);
                ShowCursor(false);
            }
            else if (interaction_mode == InteractionMode::EDITOR)
            {
                ShowCursor(true);
            }
        }


        _eventQueue->PushEvent(std::make_shared<EventVariant>(MouseMovedEvent(_cursorPos.x, _cursorPos.y)));
        break;
    }

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    {
        MouseCode button = Mouse::None;
        switch (msg)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            button = Mouse::Button0;
            break;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            button = Mouse::Button1;
            break;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            button = Mouse::Button2;
            break;
        }

        if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN)
        {
            _eventQueue->PushEvent(std::make_shared<EventVariant>(MouseButtonPressedEvent(button)));
        }
        else
        {
            _eventQueue->PushEvent(std::make_shared<EventVariant>(MouseButtonReleasedEvent(button)));
        }
        break;
    }

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
    case WM_KEYUP:
    {
        bool isKeyDown = (msg == WM_KEYDOWN);
        int window_keycode = static_cast<int>(wparam);
        KeyCode crossplatform_keycode = GetKeyCode(window_keycode);

        if (isKeyDown)
        {
            int repeatCount = (lparam >> 16) & 0xFF;
            bool firstPress = !(lparam & (1 << 30));
            pressed_keys[crossplatform_keycode] = firstPress ? Input::KeyState::KEY_PRESSED : Input::KeyState::KEY_REPEAT;
            _eventQueue->PushEvent(std::make_shared<EventVariant>(KeyPressedEvent(crossplatform_keycode, repeatCount)));
        }
        else
        {
            pressed_keys[crossplatform_keycode] = Input::KeyState::KEY_RELEASED;
            _eventQueue->PushEvent(std::make_shared<EventVariant>(KeyReleasedEvent(crossplatform_keycode)));
        }
        break;
    }

    case WM_CLOSE:
        _eventQueue->PushEvent(std::make_shared<EventVariant>(WindowCloseEvent()));
        break;

    default:
    {
        break;
    }
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowWin::WindowWin(const WindowProps& props, EventQueue& eventQueue)
    : _eventQueue(dynamic_cast<EventQueueWin*>(&eventQueue))
    , _hinstance(GetModuleHandle(nullptr))
    , _props(props)
    , _lastMouse{Input::MOUSE_NONE, Mouse::None}
    , _cursorPos{0.f, 0.f}
    , _winThread{}
    , _winThreadID(nullptr)
    , _onThreadCreated(CreateEvent(nullptr, FALSE, FALSE, nullptr))
    , isResizing(false)
    , isPainted(true)
    , _currentWidth(props.Width)
    , _currentHeigth(props.Height)
    , _xPos(props.x)
    , _yPos(props.y)
    , _prevCursorPos(_cursorPos)
    , pressed_keys{Input::KeyState::KEY_NONE}
    , interaction_mode(InteractionMode::GAMING)
    , is_first_launch(true)
    , is_in_focus(true)
{
    POINT cursorPos;
    ::GetCursorPos(&cursorPos);
    _cursorPos.x = cursorPos.x;
    _cursorPos.y = cursorPos.y;
    _prevCursorPos = _cursorPos;

    _winThread = CreateThread(nullptr, 0, WinThreadMain, this, 0, _winThreadID);
    WaitForSingleObject(_onThreadCreated, INFINITE);
}

void WindowWin::Update()
{
    if (isResizing || _props.mode == MINIMIZED)
    {
        FU_CORE_INFO("stop rendering");
        return;
    }
    _eventQueue->PushEvent(std::make_shared<EventVariant>(AppRenderEvent()));
}

void WindowWin::SetVSync(bool enabled)
{
    UNUSED(enabled);
}

bool WindowWin::IsVSync() const
{
    return true;
}

const void* WindowWin::GetNativeHandle() const
{
    return _hwnd;
}

Input::KeyState WindowWin::GetKeyState(KeyCode keyCode) const
{
    return pressed_keys[keyCode];
}

Input::MouseState WindowWin::GetMouseState(MouseCode mouseCode) const
{
    return _lastMouse.mouseCode == mouseCode ? _lastMouse.state : Input::MouseState::MOUSE_NONE;
}

void WindowWin::GetMousePos(OUT float& xPos, OUT float& yPos) const
{
    xPos = _cursorPos.x;
    yPos = _cursorPos.y;
}

std::unique_ptr<Window> Window::CreateAppWindow(const WindowProps& props, EventQueue& eventQueue)
{
    return std::make_unique<WindowWin>(props, eventQueue);
}

void WindowWin::SetMousePos(float x, float y)
{
    _prevCursorPos = _cursorPos;
    _cursorPos.x = x;
    _cursorPos.y = y;
}

void WindowWin::SetWindowMode(WPARAM mode)
{
    switch (mode)
    {
    case SIZE_MINIMIZED:
        _props.mode = MINIMIZED;
        break;
    case SIZE_MAXIMIZED:
        _props.mode = MAXIMIZED;
        break;
    case SIZE_RESTORED:
        _props.mode = RESTORED;
        break;
    }
}
}  // namespace Fuego
