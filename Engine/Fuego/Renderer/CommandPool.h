#pragma once
#include <memory>

namespace Fuego::Renderer
{
class CommandBuffer;

class CommandPool
{
public:
    static std::unique_ptr<CommandPool> CreateCommandPool();

    virtual ~CommandPool() = default;

    virtual std::unique_ptr<CommandBuffer> CreateCommandBuffer() = 0;
};
}  // namespace Fuego::Renderer
