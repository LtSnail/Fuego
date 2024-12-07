#pragma once

#include "Renderer/Swapchain.h"
#include "Renderer/Texture.h"

namespace Fuego::Renderer
{
class SwapchainOpenGL : public Swapchain
{
public:
    virtual ~SwapchainOpenGL() = default;

    virtual std::shared_ptr<Texture> GetTexture() override;

    static std::unique_ptr<Swapchain> CreateSwapChain(); 

    protected:
    SwapchainOpenGL();
};
}