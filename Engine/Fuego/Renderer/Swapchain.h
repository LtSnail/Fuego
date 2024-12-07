#pragma once
#include <memory>

namespace Fuego::Renderer
{
class Texture;

class Swapchain
{
public:
    virtual ~Swapchain() = default;

    virtual std::shared_ptr<Texture> GetTexture() = 0;

    static std::unique_ptr<Swapchain> CreateSwapChain();
};
}  // namespace Fuego::Renderer
