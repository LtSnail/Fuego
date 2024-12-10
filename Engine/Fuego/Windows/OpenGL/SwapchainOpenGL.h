#pragma once

#include "Renderer/Swapchain.h"
#include "SurfaceOpenGL.h"

namespace Fuego::Renderer
{
class Surface;

class SwapchainOpenGL : public Swapchain
{
public:
    virtual ~SwapchainOpenGL() override;

    virtual void Present() override;

    virtual Surface& GetScreenTexture() override;

private:
    SurfaceOpenGL _surface;
    HGLRC _ctx;

protected:
    friend class DeviceOpenGL;
    SwapchainOpenGL(const Surface& surface);
};
}  // namespace Fuego::Renderer
