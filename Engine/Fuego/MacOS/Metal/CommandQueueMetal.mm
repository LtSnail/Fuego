
#include "CommandQueueMetal.h"

namespace Fuego::Renderer
{
CommandQueueMetal::CommandQueueMetal(MTL::CommandQueue* commandQueue)
    : _commandQueue(commandQueue)
{
}

}  // namespace Fuego
