#pragma once

#include "Renderer/Swapchain.h"
#include "Renderer/Texture.h"

namespace Fuego::Renderer
{
class Surface;

class SwapchainOpenGL : public Swapchain
{
public:
    SwapchainOpenGL(const Surface& surface);
    virtual ~SwapchainOpenGL() override = default;

    virtual std::shared_ptr<Texture> GetTexture() override;
};
}  // namespace Fuego::Renderer
