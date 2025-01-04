#pragma once
#include <memory>

namespace Fuego::Renderer
{
class Surface;

class Swapchain
{
public:
    virtual ~Swapchain() = default;

    virtual void Present() = 0;

    virtual void ShowWireFrame(bool show) = 0;
    virtual void ValidateWindow() = 0;

    virtual Surface& GetScreenTexture() = 0;  // TODO: not sure if we should return a surface
};
}  // namespace Fuego::Renderer
