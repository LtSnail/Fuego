#pragma once

namespace Fuego::Renderer
{
class Texture;

class Swapchain
{
public:
    virtual ~Swapchain() = default;

    virtual std::shared_ptr<Texture> GetTexture() = 0;
};
}  // namespace Fuego::Renderer
