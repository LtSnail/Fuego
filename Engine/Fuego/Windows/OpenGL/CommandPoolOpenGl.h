#pragma once

#include "Renderer/CommandPool.h"

namespace Fuego::Renderer
{
class CommandQueue;
class CommandBufferOpenGL;

class CommandPoolOpenGL : public CommandPool
{
public:
    virtual ~CommandPoolOpenGL() override;

    virtual CommandBuffer& GetCommandBuffer() override;

protected:
    friend class DeviceOpenGL;
    CommandPoolOpenGL(const CommandQueue& queue);

private:
    std::vector<CommandBufferOpenGL*> _cmdBuffers;
};
}  // namespace Fuego::Renderer
