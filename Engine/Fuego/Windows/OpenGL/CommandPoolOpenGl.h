#pragma once

#include "Renderer/CommandPool.h"

namespace Fuego::Renderer
{
class CommandQueue;
class CommandBufferOpenGL;

class CommandPoolOpenGL final : public CommandPool
{
public:
    FUEGO_NON_COPYABLE_NON_MOVABLE(CommandPoolOpenGL)

    virtual ~CommandPoolOpenGL() override = default;

    virtual CommandBuffer& GetCommandBuffer() override;

protected:
    friend class DeviceOpenGL;
    explicit CommandPoolOpenGL(const CommandQueue& queue);

private:
    std::vector<std::shared_ptr<CommandBufferOpenGL>> _cmdBuffers;
};
}  // namespace Fuego::Renderer
