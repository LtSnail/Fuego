#pragma once

#include "Renderer/Swapchain.h"
#include "Renderer/Texture.h"
#include "SurfaceWindows.h"

namespace Fuego::Renderer
{
class Surface;

class SwapchainOpenGL : public Swapchain
{
public:
    SwapchainOpenGL(const Surface& surface);
    ~SwapchainOpenGL() override;

    virtual void Present() override;

    virtual std::shared_ptr<Texture> GetTexture() override;

private:
    SurfaceWindows _surface;
    HGLRC _ctx;
};
}  // namespace Fuego::Renderer
