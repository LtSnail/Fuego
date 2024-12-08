#pragma once

#include "Renderer/CommandQueue.h"

namespace Fuego::Renderer
{
class CommandQueueOpenGL : public CommandQueue
{
public:
    CommandQueueOpenGL();
    virtual ~CommandQueueOpenGL() = default;

    virtual void Submit(const CommandBuffer& commandBuffer) override;
    virtual void Wait() override;


protected:
};
}  // namespace Fuego::Renderer
