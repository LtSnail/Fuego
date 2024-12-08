
#pragma once

#include "Renderer/CommandQueue.h"

#include <Metal/Metal.hpp>

namespace Fuego::Renderer
{
class CommandQueueMetal : public CommandQueue
{
public:
    CommandQueueMetal(MTL::CommandQueue* commandQueue);
    
    virtual void Submit(const CommandBuffer& commandBuffer) {}
    virtual void Wait() {}
    
private:
    MTL::CommandQueue* _commandQueue;
};
}  // namespace Fuego
