#pragma once

#include "Renderer/Swapchain.h"
#include "SurfaceOpenGL.h"

namespace Fuego::Renderer
{
class Surface;

class SwapchainOpenGL : public Swapchain
{
public:
    SwapchainOpenGL(const Surface& surface);
    ~SwapchainOpenGL() override;

    virtual void Present() override;

    virtual Surface& GetScreenTexture() override;

private:
    SurfaceOpenGL _surface;
    HGLRC _ctx;
};
}  // namespace Fuego::Renderer
