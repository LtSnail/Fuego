#pragma once

#include "MaterialOpenGL.h"
#include "ShaderObject.h"
#include "ShaderOpenGl.h"

namespace Fuego::Graphics
{
class ShaderObjectOpenGL : public ShaderObject
{
public:
    virtual ~ShaderObjectOpenGL() override;

    inline virtual Shader* GetPixelShader() override
    {
        return pixel_shader.get();
    }
    inline virtual Shader* GetVertexShader() override
    {
        return vertex_shader.get();
    }
    inline virtual uint32_t GetObjectID()
    {
        return program;
    }

    virtual void Use() const override;
    virtual void BindMaterial(Material* material) override;

    virtual void Release() override;

private:
    MaterialOpenGL* material;
    uint32_t program;

    std::unique_ptr<ShaderOpenGL> vertex_shader;
    std::unique_ptr<ShaderOpenGL> pixel_shader;

    friend class ShaderObject;
    ShaderObjectOpenGL(Shader* vs, Shader* px);
};
}  // namespace Fuego::Graphics
