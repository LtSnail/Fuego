#include "OpenGL/DeviceOpenGL.h"

#include "Application.h"
#include "BufferOpenGL.h"
#include "CommandPoolOpenGL.h"
#include "CommandQueueOpenGL.h"
#include "Core.h"
#include "ShaderOpenGL.h"
#include "SurfaceWindows.h"
#include "SwapchainOpenGL.h"
#include "glad/glad.h"


namespace Fuego::Renderer
{
DeviceOpenGL::DeviceOpenGL(const Surface& surface)
    : _ctx(nullptr)
{
    const SurfaceWindows& surfaceWin = dynamic_cast<const SurfaceWindows&>(surface);
    const HDC hdc = surfaceWin.GetHDC();
    int pixelFormat = ChoosePixelFormat(hdc, surfaceWin.GetPFD());
    SetPixelFormat(hdc, pixelFormat, surfaceWin.GetPFD());
    _ctx = wglCreateContext(hdc);
    wglMakeCurrent(hdc, _ctx);
    if (!gladLoadGL())
        FU_CORE_ASSERT(_ctx, "[OpenGL] hasn't been initialized!");

    FU_CORE_INFO("OpenGL info:");
    FU_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
    FU_CORE_INFO("  GLSL Version: {0}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    FU_CORE_INFO("  GPU Vendor: {0}", (const char*)glGetString(GL_VENDOR));
    FU_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
}

std::unique_ptr<Device> Device::CreateDevice(const Surface& surface)
{
    return std::make_unique<DeviceOpenGL>(surface);
}

DeviceOpenGL::~DeviceOpenGL()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(_ctx);
}

std::unique_ptr<Buffer> DeviceOpenGL::CreateBuffer(size_t size, uint32_t flags)
{
    return std::make_unique<BufferOpenGL>(size, flags);
}

std::unique_ptr<CommandQueue> DeviceOpenGL::CreateQueue()
{
    return std::make_unique<CommandQueueOpenGL>();
}

std::unique_ptr<CommandPool> DeviceOpenGL::CreateCommandPool(const CommandQueue& queue)
{
    return std::make_unique<CommandPoolOpenGL>(queue);
}

std::unique_ptr<Swapchain> DeviceOpenGL::CreateSwapchain(const Surface& surface)
{
    return std::make_unique<SwapchainOpenGL>(surface);
}

std::unique_ptr<Shader> DeviceOpenGL::CreateShader(std::string_view shaderName)
{
    static constexpr const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 position; // Vertex position
        layout(location = 1) in vec3 color;    // Vertex color

        out vec3 fragColor; // Pass color to fragment shader

        void main()
        {
            gl_Position = vec4(position, 1.0); // Transform vertex to clip space
            fragColor = color;                // Pass color to the fragment shader
        }
    )";

    static constexpr const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 fragColor;   // Interpolated color from the vertex shader
        out vec4 color;      // Output color to the framebuffer

        void main()
        {
            color = vec4(fragColor, 1.0); // Set fragment color
        }
    )";

    // Temporary code:
    if (shaderName == "vs_triangle")
    {
        return std::make_unique<ShaderOpenGL>(vertexShaderSource, Shader::ShaderType::Vertex);
    }

    if (shaderName == "ps_triangle")
    {
        return std::make_unique<ShaderOpenGL>(fragmentShaderSource, Shader::ShaderType::Pixel);
    }

    return nullptr;
}

}  // namespace Fuego::Renderer
