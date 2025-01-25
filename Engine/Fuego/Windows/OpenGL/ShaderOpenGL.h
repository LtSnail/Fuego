#pragma once

#include "Renderer/Shader.h"

namespace Fuego::Renderer
{
class ShaderOpenGL final : public Shader
{
public:
    FUEGO_NON_COPYABLE_NON_MOVABLE(ShaderOpenGL)

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

    friend class DeviceOpenGL;
    ShaderOpenGL(const char* shaderCode, ShaderType type);
};
}  // namespace Fuego::Renderer
