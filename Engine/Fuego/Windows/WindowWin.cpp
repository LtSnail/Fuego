#include "WindowWin.h"

#include <glad/gl.h>
#include <glad/wgl.h>

#include "InputWin.h"
#include "Log.h"

#define LAST_CODE UINT16_MAX

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int* attribList);
wglCreateContextAttribsARB_type* wglCreateContextAttribsARB;

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

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
    sprintf_s(buffer, window->_props.Title.c_str());
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
    while (GetMessage(&msg, nullptr, 0u, 0u))
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
    WindowWin* window = reinterpret_cast<WindowWin*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (window)
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
        .hInstance = GetModuleHandle(0),
        .lpszClassName = "Dummy_Window",
    };

    if (!RegisterClassA(&window_class))
        FU_CORE_ERROR("Failed to register dummy OpenGL window");

    HWND dummy_window = CreateWindowExA(0, window_class.lpszClassName, "Dummy OpenGL Window", 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0,
                                        0, window_class.hInstance, 0);
    if (!dummy_window)
        FU_CORE_ERROR("Failed to create dummy OpenGL window.");

    HDC dummy_dc = GetDC(dummy_window);

    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
    if (!pixel_format)
        FU_CORE_ERROR("Failed to find a suitable pixel format.");
    if (!SetPixelFormat(dummy_dc, pixel_format, &pfd))
        FU_CORE_ERROR("Failed to set the pixel format.");

    HGLRC dummy_context = wglCreateContext(dummy_dc);
    if (!dummy_context)
        FU_CORE_ERROR("Failed to create a dummy OpenGL rendering context.");

    if (!wglMakeCurrent(dummy_dc, dummy_context))
        FU_CORE_ERROR("Failed to activate dummy OpenGL rendering context.");

    wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");

    wglMakeCurrent(dummy_dc, 0);
    wglDeleteContext(dummy_context);
    ReleaseDC(dummy_window, dummy_dc);
    DestroyWindow(dummy_window);
}

LRESULT WindowWin::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_ACTIVATE:
        break;
    case WM_PAINT:
    {
        if (!isPainted || isResizing || _props.mode == MINIMIZED) return 0;
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

        UINT width = LOWORD(lparam);
        UINT height = HIWORD(lparam);
        _currentWidth = width;
        _currentHeigth = height;
        _eventQueue->PushEvent(std::make_shared<EventVariant>(WindowResizeEvent(width, height)));
        break;
    }
    case WM_SIZE:
    {
        if (isResizing) break;

        UINT width = LOWORD(lparam);
        UINT height = HIWORD(lparam);
        _currentWidth = width;
        _currentHeigth = height;
        _eventQueue->PushEvent(std::make_shared<EventVariant>(WindowResizeEvent(width, height)));

        SetWindowMode(wparam);
        break;
    }
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
    , _hinstance(GetModuleHandle(nullptr))
    , _props(props)
    , _lastKey{Input::KEY_NONE, LAST_CODE}
    , _lastMouse{Input::MOUSE_NONE, LAST_CODE}
    , _cursorPos{0.f, 0.f}
    , _winThread{}
    , _winThreadID(nullptr)
    , _onThreadCreated(CreateEvent(nullptr, FALSE, FALSE, nullptr))
    , isResizing(false)
    , isPainted(true)
    , _currentWidth(props.Width)
    , _currentHeigth(props.Height)
{
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

std::unique_ptr<Window> Window::CreateAppWindow(const WindowProps& props, EventQueue& eventQueue)
{
    return std::make_unique<WindowWin>(props, eventQueue);
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
