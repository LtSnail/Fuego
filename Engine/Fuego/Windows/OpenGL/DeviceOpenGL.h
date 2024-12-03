#pragma once
#include <memory>

#include "Renderer/Device.h"

namespace Fuego::Renderer
{
class DeviceOpenGL : public Device
{
public:
    virtual ~DeviceOpenGL() override;

    virtual std::unique_ptr<Buffer> CreateBuffer(size_t size, uint32_t flags) override;
    virtual std::unique_ptr<CommandQueue> CreateQueue() override;
    virtual std::unique_ptr<CommandPool> CreateCommandPool(std::shared_ptr<CommandQueue> queue) override;
    virtual std::unique_ptr<Swapchain> CreateSwapchain(std::shared_ptr<Surface> surface) override;

    static std::unique_ptr<Device> CreateDevice(Surface* surface);

protected:
    DeviceOpenGL();

private:
    HWND* _hwnd;                 // WIN32: Window descriptor
    HGLRC _ctx;                  // OpenGL: context
    HDC _hdc;                    // WIN32: Handle to Device Context
    PIXELFORMATDESCRIPTOR _pfd;  // WIN32: Pixel format descriptor
};
}  // namespace Fuego::Renderer
