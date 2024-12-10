#pragma once
#include <memory>

namespace Fuego::Renderer
{
class CommandBuffer;
class CommandQueue;

class CommandPool
{
public:
    virtual ~CommandPool() = default;

    virtual std::unique_ptr<CommandBuffer> CreateCommandBuffer() = 0;
};
}  // namespace Fuego::Renderer
