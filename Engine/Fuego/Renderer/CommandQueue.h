#pragma once

#include <memory>

namespace Fuego::Renderer
{
class CommandBuffer;

class CommandQueue
{
public:
    static std::unique_ptr<CommandQueue> CreateQueue();
    virtual ~CommandQueue() = default;

    virtual void Submit(std::shared_ptr<CommandBuffer> commandBuffer) = 0;
    virtual void Wait() = 0;
};
}  // namespace Fuego::Renderer
