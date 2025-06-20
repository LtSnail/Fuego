#pragma once

#include "MaterialOpenGL.h"
#include "ShaderObject.h"
#include "ShaderOpenGl.h"

namespace Fuego::Graphics
{
class ShaderObjectOpenGL : public ShaderObject
{
public:
    friend class ShaderObject;

    virtual ~ShaderObjectOpenGL() override;

    inline virtual uint32_t GetObjectID()
    {
        return program;
    }

    virtual void Use() const override;
    bool AddVar(std::string_view uniform_name, uint32_t id);
    virtual void BindMaterial(const Material* material) override;

    virtual void Release() override;

private:
    const MaterialOpenGL* material;
    uint32_t program;

    std::unique_ptr<ShaderOpenGL> vertex_shader;
    std::unique_ptr<ShaderOpenGL> pixel_shader;

    ShaderObjectOpenGL(Shader* vs, Shader* px);

private:
    std::unordered_map<std::string, uint32_t> uniforms;

    uint32_t find_uniform_location(std::string_view uniform_name) const;

protected:
    virtual bool set_vec3f_impl(std::string_view uniform_name, const glm::vec3& vec) override;
    virtual bool set_mat4f_impl(std::string_view uniform_name, const glm::mat4& matrix) override;
    virtual bool set_text2d_impl(std::string_view uniform_name, const Texture& texture) override;
};
}  // namespace Fuego::Graphics
