#include "Buffer.h"

#include "Renderer.h"

namespace Fuego
{
VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
{
    UNUSED(vertices);
    UNUSED(size);

    switch (Renderer::GetAPI())
    {
        case RendererAPI::OpenGL:
            return nullptr;

        case RendererAPI::Metal:
            return nullptr;

        case RendererAPI::DerectX12:
            return nullptr;

        case RendererAPI::None:
        default:
            FU_CORE_ASSERT(false, "[Renderer] Wrong Renderer");
            return nullptr;
    }
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
{
    UNUSED(indices);
    UNUSED(size);

    switch (Renderer::GetAPI())
    {
        case RendererAPI::OpenGL:
            return nullptr;

        case RendererAPI::Metal:
            return nullptr;

        case RendererAPI::DerectX12:
            return nullptr;

        case RendererAPI::None:
        default:
            FU_CORE_ASSERT(false, "[Renderer] Wrong Renderer");
            return nullptr;
    }
}
}  // namespace Fuego