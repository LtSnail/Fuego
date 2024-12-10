#pragma once

#include "Renderer/CommandPool.h"

namespace Fuego::Renderer
{
class CommandQueue;

class CommandPoolOpenGL : public CommandPool
{
public:
    virtual ~CommandPoolOpenGL() = default;

    virtual std::unique_ptr<CommandBuffer> CreateCommandBuffer() override;

protected:
    friend class DeviceOpenGL;
    CommandPoolOpenGL(const CommandQueue& queue);
};
}  // namespace Fuego::Renderer
