#pragma once

#include "Renderer/CommandQueue.h"

namespace Fuego::Renderer
{
class CommandQueueOpenGL : public CommandQueue
{
public:
    virtual ~CommandQueueOpenGL() = default;

    virtual void Submit(const CommandBuffer& commandBuffer) override;
    virtual void Wait() override;


protected:
    friend class DeviceOpenGL;
    CommandQueueOpenGL();
};
}  // namespace Fuego::Renderer
