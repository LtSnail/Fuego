#pragma once

#include "Renderer/CommandPool.h"

namespace Fuego::Renderer
{
class CommandQueue;

class CommandPoolOpenGL : public CommandPool
{
public:
    CommandPoolOpenGL(const CommandQueue& queue);
    ~CommandPoolOpenGL();

    virtual std::unique_ptr<CommandBuffer> CreateCommandBuffer() override;
};
}  // namespace Fuego::Renderer
