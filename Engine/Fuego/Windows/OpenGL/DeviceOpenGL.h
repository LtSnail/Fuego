#pragma once
#include <memory>

#include "Renderer/Device.h"
#include "glad/gl.h"

namespace Fuego::Renderer
{
class DeviceOpenGL : public Device
{
public:
    virtual ~DeviceOpenGL();

    virtual std::unique_ptr<Buffer> CreateBuffer(size_t size, uint32_t flags) override;
    virtual std::unique_ptr<CommandQueue> CreateCommandQueue() override;
    virtual std::unique_ptr<CommandPool> CreateCommandPool(const CommandQueue& queue) override;

    virtual std::unique_ptr<Swapchain> CreateSwapchain(const Surface& surface) override;
    virtual std::unique_ptr<Swapchain> CreateSwapchain(const Surface& surface, float x, float y, float w, float h) override;

    virtual std::unique_ptr<Shader> CreateShader(std::string_view shaderName, Shader::ShaderType type) override;
    virtual std::unique_ptr<Surface> CreateSurface(const void* window) override;

protected:
    friend class Device;
    DeviceOpenGL();

private:
    static void APIENTRY OpenGLDebugCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                                                 const void* userParam);
    HGLRC ctx;
    int max_textures_units;
};
}  // namespace Fuego::Renderer
