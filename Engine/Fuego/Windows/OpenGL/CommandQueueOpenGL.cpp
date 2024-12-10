#include "CommandQueueOpenGL.h"

namespace Fuego::Renderer
{

CommandQueueOpenGL::CommandQueueOpenGL()
{
}

void CommandQueueOpenGL::Submit(const CommandBuffer& commandBuffer)
{
    UNUSED(commandBuffer);
}

void CommandQueueOpenGL::Wait()
{
}

}  // namespace Fuego::Renderer
