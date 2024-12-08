#include "CommandQueueOpenGL.h"

namespace Fuego::Renderer
{

CommandQueueOpenGL::CommandQueueOpenGL(const CommandQueue& queue)
{
    UNUSED(queue);
}

void CommandQueueOpenGL::Submit(const CommandBuffer& commandBuffer)
{
    UNUSED(commandBuffer);
}

void CommandQueueOpenGL::Wait()
{
}

std::unique_ptr<CommandQueue> CommandQueue::CreateQueue()
{
    return std::unique_ptr<CommandQueueOpenGL>();
}

}  // namespace Fuego::Renderer
