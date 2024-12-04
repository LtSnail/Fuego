#pragma once

#include "Renderer/Shader.h"

#include <Metal/Metal.hpp>

namespace Fuego::Renderer
{
class ShaderMetal : public Shader
{
public:
    friend class CommandBufferMetal;
    
    ShaderMetal(MTL::Function* function);
    
private:
    MTL::Function* _function;
};
}  // namespace Fuego
