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

    virtual void ShowWireFrame(bool show) override;
    virtual void ValidateWindow() override;

private:
    SurfaceOpenGL _surface;

protected:
    friend class DeviceOpenGL;
    SwapchainOpenGL(const Surface& surface);
    SwapchainOpenGL(const Surface& surface, float x, float y, float w, float h);
};
}  // namespace Fuego::Renderer
