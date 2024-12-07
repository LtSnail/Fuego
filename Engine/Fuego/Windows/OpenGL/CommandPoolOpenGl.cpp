#include "CommandPoolOpenGl.h"
#include "CommandBufferOpenGL.h"

namespace Fuego::Renderer
{

CommandPoolOpenGl::CommandPoolOpenGl()
{
}

std::unique_ptr<CommandPool> CommandPoolOpenGl::CreateCommandPool()
{
    return std::unique_ptr<CommandPoolOpenGl>();
}

CommandPoolOpenGl::~CommandPoolOpenGl()
{
}

std::unique_ptr<CommandBuffer> CommandPoolOpenGl::CreateCommandBuffer()
{
    return CommandBuffer::CreateCommandBuffer();
}

}
