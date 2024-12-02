#pragma once

namespace Fuego::Renderer
{
class CommandBuffer;

class CommandPool
{
public:
    virtual ~CommandPool() = default;

    virtual std::unique_ptr<CommandBuffer> CreateCommandBuffer() = 0;
};
}  // namespace Fuego::Renderer
