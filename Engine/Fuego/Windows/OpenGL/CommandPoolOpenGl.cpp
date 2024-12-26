#include "CommandPoolOpenGl.h"

#include "CommandBufferOpenGL.h"

namespace Fuego::Renderer
{

CommandPoolOpenGL::CommandPoolOpenGL(const CommandQueue& queue)
    : _cmdBuffers()
{
    UNUSED(queue);
    _cmdBuffers.emplace_back(new CommandBufferOpenGL());
    _cmdBuffers.emplace_back(new CommandBufferOpenGL());
    _cmdBuffers.shrink_to_fit();
}

CommandPoolOpenGL::~CommandPoolOpenGL()
{
    for (auto ptr : _cmdBuffers)
    {
        delete ptr;
    }
}

CommandBuffer& CommandPoolOpenGL::GetCommandBuffer()
{
    for (auto cmd : _cmdBuffers)
    {
        if (cmd->_isFree)
            return *cmd;
    }
}

}  // namespace Fuego::Renderer
