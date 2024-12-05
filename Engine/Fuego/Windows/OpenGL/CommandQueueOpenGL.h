#pragma once

#include "Renderer/CommandQueue.h"

namespace Fuego::Renderer
{
class CommandQueueOpenGL : public CommandQueue
{
public:
    virtual ~CommandQueueOpenGL() = default;

    virtual void Submit(std::shared_ptr<CommandBuffer> commandBuffer) override;
    virtual void Wait() override;

    static std::unique_ptr<CommandQueue> CreateQueue();

protected:
    CommandQueueOpenGL();
};
}  // namespace Fuego::Renderer
