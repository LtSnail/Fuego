#pragma once

#include <cstdint>
#include <glm/fwd.hpp>

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
    virtual bool AddVar(const std::string& uniform) override;
    virtual bool SetVec3f(const std::string& var, glm::vec3 vector) const override;
    virtual bool SetMat4f(const std::string& var, glm::mat4 matrix) const override;
    virtual bool SetText2D(const std::string& var, const Texture& texture) const override;

    virtual void Release() override;

private:
    uint32_t shader_object;
    uint32_t _shaderID;
    ShaderType _type;

    std::unordered_map<std::string, uint32_t> uniforms;

protected:
    friend class DeviceOpenGL;
    ShaderOpenGL(const char* shaderCode, ShaderType type);
};
}  // namespace Fuego::Graphics
