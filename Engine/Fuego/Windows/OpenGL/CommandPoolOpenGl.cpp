#include "CommandPoolOpenGl.h"

#include "CommandBufferOpenGL.h"

namespace Fuego::Renderer
{

CommandPoolOpenGL::CommandPoolOpenGL(const CommandQueue& queue)
{
    UNUSED(queue);
}

CommandPoolOpenGL::~CommandPoolOpenGL()
{
}

std::unique_ptr<CommandBuffer> CommandPoolOpenGL::CreateCommandBuffer()
{
    return CommandBuffer::CreateCommandBuffer();
}

}  // namespace Fuego::Renderer
