#include "Renderer.h"

#include "fupch.h"

namespace Fuego
{
#if defined(FUEGO_PLATFORM_WIN)
RendererAPI Renderer::_rendererAPI = RendererAPI::OpenGL;
#elif defined(FUEGO_PLATFORM_MACOS)
RendererAPI Renderer::_rendererAPI = RendererAPI::Metal;
#endif
}  // namespace Fuego