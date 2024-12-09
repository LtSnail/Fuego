#pragma once

#include <cstdint>

#include "Renderer/Shader.h"
#include "glad/glad.h"

namespace Fuego::Renderer
{
class ShaderOpenGL : public Shader
{
public:
    ShaderOpenGL(const char* shaderCode, ShaderType type);
    ~ShaderOpenGL();
    inline ShaderType GetType() const
    {
        return _type;
    }
    inline uint32_t GetID() const
    {
        return _shaderID;
    }

private:
    uint32_t _shaderID;
    ShaderType _type;

    GLint GetShaderType(ShaderType type) const;
};
}  // namespace Fuego::Renderer
