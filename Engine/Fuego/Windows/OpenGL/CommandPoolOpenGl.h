#pragma once

#include "Renderer/CommandPool.h"

namespace Fuego::Renderer
{
class CommandQueue;

class CommandPoolOpenGl : public CommandPool
{
public:
    CommandPoolOpenGl(const CommandQueue& queue);
    virtual ~CommandPoolOpenGl() override;
    virtual std::unique_ptr<CommandBuffer> CreateCommandBuffer() override;
};
}  // namespace Fuego::Renderer
