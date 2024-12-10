#pragma once

#include <cstdint>

#include "Renderer/Shader.h"
#include "glad/glad.h"

namespace Fuego::Renderer
{
class ShaderOpenGL : public Shader
{
public:
    virtual ~ShaderOpenGL() override;

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

protected:
    friend class DeviceOpenGL;
    ShaderOpenGL(const char* shaderCode, ShaderType type);
};
}  // namespace Fuego::Renderer
