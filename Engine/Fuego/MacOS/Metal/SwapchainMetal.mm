#include "SwapchainMetal.h"

#include "Metal/SurfaceMetal.h"
#include "TextureMetal.h"

namespace Fuego::Renderer
{
SwapchainMetal::SwapchainMetal(const Surface& surface)
    : _surface(dynamic_cast<const SurfaceMetal&>(surface))
{
}

SwapchainMetal::~SwapchainMetal()
{
}

void SwapchainMetal::Present()
{
}

Surface& SwapchainMetal::GetScreenTexture()
{
    return _surface;
}
}  // namespace Fuego::Renderer
