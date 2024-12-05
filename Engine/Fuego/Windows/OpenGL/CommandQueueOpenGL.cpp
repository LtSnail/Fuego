#include "CommandQueueOpenGL.h"

namespace Fuego::Renderer
{

CommandQueueOpenGL::CommandQueueOpenGL()
{
}

void CommandQueueOpenGL::Submit(std::shared_ptr<CommandBuffer> commandBuffer)
{
    UNUSED(commandBuffer);
}

void CommandQueueOpenGL::Wait()
{
}

std::unique_ptr<CommandQueue> CommandQueueOpenGL::CreateQueue()
{
    return std::unique_ptr<CommandQueueOpenGL>();
}

}  // namespace Fuego::Renderer
