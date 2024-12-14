#include "SwapchainOpenGL.h"

#include "OpenGL/SurfaceOpenGL.h"
#include "TextureOpenGL.h"
#include "glad/gl.h"
#include "glad/wgl.h"

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int* attribList);
wglCreateContextAttribsARB_type* wglCreateContextAttribsARB;

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats,
                                                 UINT* nNumFormats);
wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

namespace Fuego::Renderer
{
SwapchainOpenGL::SwapchainOpenGL(const Surface& surface)
    : _surface(dynamic_cast<const SurfaceOpenGL&>(surface))
{
    InitOpenGLExtension();
    int pixel_format_attribs[] = {WGL_DRAW_TO_WINDOW_ARB,
                                  GL_TRUE,
                                  WGL_SUPPORT_OPENGL_ARB,
                                  GL_TRUE,
                                  WGL_DOUBLE_BUFFER_ARB,
                                  GL_TRUE,
                                  WGL_ACCELERATION_ARB,
                                  WGL_FULL_ACCELERATION_ARB,
                                  WGL_PIXEL_TYPE_ARB,
                                  WGL_TYPE_RGBA_ARB,
                                  WGL_COLOR_BITS_ARB,
                                  32,
                                  WGL_DEPTH_BITS_ARB,
                                  24,
                                  WGL_STENCIL_BITS_ARB,
                                  8,
                                  0};

    int pixel_format;
    UINT num_formats;
    wglChoosePixelFormatARB(_surface.GetHdc(), pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
    FU_CORE_ASSERT(num_formats, "asd");

    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(_surface.GetHdc(), pixel_format, sizeof(pfd), &pfd);
    FU_CORE_ASSERT(SetPixelFormat(_surface.GetHdc(), pixel_format, &pfd), "qwe");
    

    int gl33_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3, WGL_CONTEXT_MINOR_VERSION_ARB, 3, WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0,
    };

    HGLRC gl33_context = wglCreateContextAttribsARB(_surface.GetHdc(), 0, gl33_attribs);

    FU_CORE_ASSERT(wglMakeCurrent(_surface.GetHdc(), gl33_context), "123");


    FU_CORE_INFO("OpenGL info:");
    FU_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    FU_CORE_INFO("  GLSL Version: {0}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    FU_CORE_INFO("  GPU Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    FU_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    // TODO: if debuf then enable OpenGL debug callback:
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(SwapchainOpenGL::OpenGLDebugCallbackFunc, nullptr);
}

SwapchainOpenGL::~SwapchainOpenGL()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(_ctx);
    _ctx = nullptr;
}

void SwapchainOpenGL::Present()
{
    static PAINTSTRUCT ps;
    BeginPaint((HWND)_surface.GetNativeHandle(), &ps);
    EndPaint((HWND)_surface.GetNativeHandle(), &ps);
    glClearColor(0.2f, 0.3f, 0.1f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);  // TODO: move from here
    SwapBuffers(_surface.GetHdc());
}

Surface& SwapchainOpenGL::GetScreenTexture()
{
    return _surface;
}

void APIENTRY SwapchainOpenGL::OpenGLDebugCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                                                       const void* userParam)
{
    FU_CORE_ERROR("[OpenGL] \nMessage: {0}\nSource: {1}\nType: {2}\nID: {3}\nSeverity: {4}", (const char*)message, source, type, id, severity);
}
void SwapchainOpenGL::InitOpenGLExtension()
{
    WNDCLASSA window_class = {
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = DefWindowProcA,
        .hInstance = GetModuleHandle(0),
        .lpszClassName = "Dummy_WGL_Extension",
    };

    FU_CORE_ASSERT(RegisterClassA(&window_class), "[OpenGl extension]");

    HWND dummy_window = CreateWindowExA(0, window_class.lpszClassName, "Dummy OpenGL Window", 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0,
                                        0, window_class.hInstance, 0);

    FU_CORE_ASSERT(dummy_window, "[Dummy Window]");

    HDC dummy_dc = GetDC(dummy_window);

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(pfd),                                                 // nSize
        1,                                                           // nVersion
        PFD_TYPE_RGBA,                                               // iPixelType
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,  // dwFlags
        32,                                                          // cColorBits
        8,                                                           // cAlphaBits
        PFD_MAIN_PLANE,                                              // iLayerType
        24,                                                          // cDepthBits
        8                                                            // cStencilBits
    };

    int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
    FU_CORE_ASSERT(pixel_format, "asd");

    FU_CORE_ASSERT(SetPixelFormat(dummy_dc, pixel_format, &pfd), "qwe");

    HGLRC dummy_context = wglCreateContext(dummy_dc);
    FU_CORE_ASSERT(dummy_context, "qwe");

    wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");

    wglMakeCurrent(dummy_dc, 0);
    wglDeleteContext(dummy_context);
    ReleaseDC(dummy_window, dummy_dc);
    DestroyWindow(dummy_window);
}
}  // namespace Fuego::Renderer
