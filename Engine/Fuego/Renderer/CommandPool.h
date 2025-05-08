#pragma once

namespace Fuego::Graphics
{
class CommandBuffer;
class CommandQueue;

class CommandPool
{
public:
    virtual ~CommandPool() = default;
};
}  // namespace Fuego::Graphics
