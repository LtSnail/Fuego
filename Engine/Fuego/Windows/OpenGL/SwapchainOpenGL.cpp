#include "SwapchainOpenGL.h"
#include "TextureStab.h"

namespace Fuego::Renderer

{

SwapchainOpenGL::SwapchainOpenGL()
{

}

std::unique_ptr<Swapchain> SwapchainOpenGL::CreateSwapChain()
{
    return std::unique_ptr<SwapchainOpenGL>();
}

std::shared_ptr<Texture> SwapchainOpenGL::GetTexture()
{
    return std::make_shared<TextureStab>();
}

}