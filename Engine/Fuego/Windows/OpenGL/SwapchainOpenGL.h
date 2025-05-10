#pragma once

#include "Renderer/Swapchain.h"
#include "SurfaceOpenGL.h"

namespace Fuego::Graphics
{
class Surface;

class SwapchainOpenGL final : public Swapchain
{
public:
    FUEGO_NON_COPYABLE_NON_MOVABLE(SwapchainOpenGL);

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
};
}  // namespace Fuego::Graphics
