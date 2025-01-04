#pragma once

namespace Fuego::Renderer
{
class CommandBuffer;

class CommandQueue
{
public:
    virtual ~CommandQueue() = default;

    virtual void Submit(const CommandBuffer& commandBuffer) = 0;
    virtual void Wait() = 0;
};
}  // namespace Fuego::Renderer
