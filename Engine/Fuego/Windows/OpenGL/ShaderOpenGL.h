#pragma once

#include "Renderer/Shader.h"

namespace Fuego::Graphics
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

    virtual void BindToShaderObject(ShaderObject& obj) override;

    virtual void Release() override;

private:
    uint32_t shader_object;
    uint32_t _shaderID;
    ShaderType _type;

protected:
    friend class DeviceOpenGL;
    ShaderOpenGL(const char* shaderCode, ShaderType type);
};
}  // namespace Fuego::Graphics
