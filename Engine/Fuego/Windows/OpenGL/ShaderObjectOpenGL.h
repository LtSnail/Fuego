#pragma once

#include "MaterialOpenGL.h"
#include "ShaderObject.h"
#include "ShaderOpenGl.h"

namespace Fuego::Renderer
{
class ShaderObjectOpenGL : public ShaderObject
{
public:
    virtual ~ShaderObjectOpenGL() override;

    inline virtual Shader* GetPixelShader() override
    {
        return pixel_shader;
    }
    inline virtual Shader* GetVertexShader() override
    {
        return vertex_shader;
    }
    inline virtual uint32_t GetObjectID()
    {
        return program;
    }

    void Use() const;

private:
    uint32_t program;
    ShaderOpenGL* vertex_shader;
    ShaderOpenGL* pixel_shader;

    friend class ShaderObject;
    ShaderObjectOpenGL(Shader& vs, Shader& px);

    friend class MaterialOpenGL;
    void UploadMaterial(Material& material) const;
};
}  // namespace Fuego::Renderer
