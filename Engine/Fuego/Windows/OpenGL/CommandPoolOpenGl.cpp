#include "CommandPoolOpenGl.h"

#include "CommandBufferOpenGL.h"

namespace Fuego::Renderer
{

CommandPoolOpenGL::CommandPoolOpenGL(const CommandQueue& queue)
{
    UNUSED(queue);
    _cmdBuffers.emplace_back(new CommandBufferOpenGL());
    _cmdBuffers.emplace_back(new CommandBufferOpenGL());
    _cmdBuffers.shrink_to_fit();
}

CommandBuffer& CommandPoolOpenGL::GetCommandBuffer()
{
    for (const auto& cmd : _cmdBuffers)
    {
        if (cmd->_isFree)
        {
            return *cmd;
        }
    }
}

}  // namespace Fuego::Renderer
