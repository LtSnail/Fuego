#include "CommandPoolOpenGl.h"

#include "CommandBufferOpenGL.h"

namespace Fuego::Renderer
{

CommandPoolOpenGl::CommandPoolOpenGl(const CommandQueue& queue)
{
    UNUSED(queue);
}

std::unique_ptr<CommandPool> CommandPool::CreateCommandPool(const CommandQueue& queue)
{
    return std::make_unique<CommandPoolOpenGl>(queue);
}

CommandPoolOpenGl::~CommandPoolOpenGl()
{
}

std::unique_ptr<CommandBuffer> CommandPoolOpenGl::CreateCommandBuffer()
{
    return CommandBuffer::CreateCommandBuffer();
}

}  // namespace Fuego::Renderer
