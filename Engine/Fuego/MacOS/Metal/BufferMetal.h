#pragma once

#include "Renderer/Buffer.h"

#include <Metal/Metal.hpp>

namespace Fuego::Renderer
{
class BufferMetal : public Buffer
{
public:
    BufferMetal(MTL::Buffer* buffer);
    
private:
    MTL::Buffer* _buffer;
};
}  // namespace Fuego
