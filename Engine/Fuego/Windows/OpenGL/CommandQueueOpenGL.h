#pragma once

#include "Renderer/CommandQueue.h"

namespace Fuego::Graphics
{
class CommandQueueOpenGL final : public CommandQueue
{
public:
    FUEGO_NON_COPYABLE_NON_MOVABLE(CommandQueueOpenGL)

    virtual void Submit(const CommandBuffer& commandBuffer) override;
    virtual void Wait() override;

protected:
    friend class DeviceOpenGL;
    CommandQueueOpenGL() = default;
};
}  // namespace Fuego::Graphics
