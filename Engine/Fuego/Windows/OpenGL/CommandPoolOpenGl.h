#pragma once

#include "Renderer/CommandPool.h"

namespace Fuego::Renderer
{

class CommandPoolOpenGl : public CommandPool
{
public:
    static std::unique_ptr<CommandPool> CreateCommandPool();
    virtual ~CommandPoolOpenGl() override;
    virtual std::unique_ptr<CommandBuffer> CreateCommandBuffer() override;

protected:
    CommandPoolOpenGl();
};
}