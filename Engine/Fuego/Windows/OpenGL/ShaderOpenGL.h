#pragma once

#include <cstdint>

#include "Renderer/Shader.h"
#include "glad/glad.h"

namespace Fuego::Renderer
{
class ShaderOpenGL : public Shader
{
public:
    virtual ~ShaderOpenGL() = default;
    virtual void Use(uint32_t ID) const;


protected:
    virtual uint32_t CompileShader(GLenum shaderType, const char* shaderCode);

private:
    uint32_t _programID;
    uint32_t _pixelID;
    uint32_t _vertexID;
    ShaderOpenGL();

    friend class CommandBufferOpenGL;
    virtual void BindPixelShader(const char* shaderCode);
    virtual void BindVertexShader(const char* shaderCode);
};
}  // namespace Fuego::Renderer
