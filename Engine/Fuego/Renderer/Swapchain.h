#pragma once
#include <memory>

namespace Fuego::Renderer
{
class Texture;
class Surface;

class Swapchain
{
public:
    virtual ~Swapchain() = default;

    virtual std::shared_ptr<Texture> GetTexture() = 0;

    static std::unique_ptr<Swapchain> CreateSwapChain(const Surface& surface);
};
}  // namespace Fuego::Renderer
