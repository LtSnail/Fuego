#include "OpenGL/DeviceOpenGL.h"

#include "Application.h"
#include "BufferOpenGL.h"
#include "CommandPoolOpenGL.h"
#include "CommandQueueOpenGL.h"
#include "ShaderOpenGL.h"
#include "SwapchainOpenGL.h"


namespace Fuego::Renderer
{

std::unique_ptr<Device> Device::CreateDevice()
{
    return std::unique_ptr<Device>(new DeviceOpenGL());
}

DeviceOpenGL::~DeviceOpenGL()
{
}

std::unique_ptr<Buffer> DeviceOpenGL::CreateBuffer(size_t size, uint32_t flags)
{
    return std::unique_ptr<Buffer>(new BufferOpenGL(size, flags));
}

std::unique_ptr<CommandQueue> DeviceOpenGL::CreateQueue()
{
    return std::unique_ptr<CommandQueue>(new CommandQueueOpenGL());
}

std::unique_ptr<CommandPool> DeviceOpenGL::CreateCommandPool(const CommandQueue& queue)
{
    return std::unique_ptr<CommandPoolOpenGL>(new CommandPoolOpenGL(queue));
}

std::unique_ptr<Swapchain> DeviceOpenGL::CreateSwapchain(const Surface& surface)
{
    return std::unique_ptr<Swapchain>(new SwapchainOpenGL(surface));
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
        return std::unique_ptr<Shader>(new ShaderOpenGL(vertexShaderSource, Shader::ShaderType::Vertex));
    }

    if (shaderName == "ps_triangle")
    {
        return std::unique_ptr<Shader>(new ShaderOpenGL(fragmentShaderSource, Shader::ShaderType::Pixel));
    }

    return nullptr;
}

}  // namespace Fuego::Renderer
