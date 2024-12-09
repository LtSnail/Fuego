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

    virtual void Present() = 0;

    virtual std::shared_ptr<Texture> GetTexture() = 0;
};
}  // namespace Fuego::Renderer
