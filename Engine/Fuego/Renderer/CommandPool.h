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

    virtual CommandBuffer& GetCommandBuffer() = 0;
};
}  // namespace Fuego::Renderer
