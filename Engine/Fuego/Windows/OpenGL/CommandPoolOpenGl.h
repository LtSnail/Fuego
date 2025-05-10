#pragma once

#include "Renderer/CommandPool.h"

namespace Fuego::Graphics
{
class CommandQueue;
class CommandBufferOpenGL;

class CommandPoolOpenGL final : public CommandPool
{
public:
    FUEGO_NON_COPYABLE_NON_MOVABLE(CommandPoolOpenGL)

    virtual ~CommandPoolOpenGL() override = default;

protected:
    friend class DeviceOpenGL;
    explicit CommandPoolOpenGL(const CommandQueue& queue);
};
}  // namespace Fuego::Graphics
