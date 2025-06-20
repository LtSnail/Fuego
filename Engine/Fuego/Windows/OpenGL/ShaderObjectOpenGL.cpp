#include "ShaderObjectOpenGL.h"

#include <glm/gtc/type_ptr.hpp>

#include "glad/gl.h"

namespace Fuego::Graphics
{

struct uniform_info
{
    GLint location;
    GLsizei count;
    GLenum type;
};

ShaderObject* ShaderObject::CreateShaderObject(Shader* vs, Shader* px)
{
    return new ShaderObjectOpenGL(vs, px);
}

ShaderObjectOpenGL::ShaderObjectOpenGL(Shader* vs, Shader* px)
    : program(glCreateProgram())
    , vertex_shader(nullptr)
    , pixel_shader(nullptr)
    , material(nullptr)
{
    vertex_shader.reset(static_cast<ShaderOpenGL*>(vs));
    pixel_shader.reset(static_cast<ShaderOpenGL*>(px));

    glAttachShader(program, vertex_shader->GetID());
    glAttachShader(program, pixel_shader->GetID());

    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        FU_CORE_ERROR("[ShaderObject] program linking error: ", infoLog);
    }

    GLint uniform_count = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_count);
    if (uniform_count > 0)
    {
        GLint max_name_len = 0;
        glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

        auto uniform_name = std::make_unique<char[]>(max_name_len);

        for (GLint i = 0; i < uniform_count; ++i)
        {
            GLsizei length = 0;
            uniform_info info = {};
            glGetActiveUniform(program, i, max_name_len, &length, &info.count, &info.type, uniform_name.get());
            info.location = glGetUniformLocation(program, uniform_name.get());

            AddVar(std::string(uniform_name.get(), length), info.location);
        }
    }
    vertex_shader->BindToShaderObject(*this);
    pixel_shader->BindToShaderObject(*this);

    glUseProgram(0);
}

ShaderObjectOpenGL::~ShaderObjectOpenGL()
{
    Release();
}

void ShaderObjectOpenGL::Release()
{
    glDeleteProgram(program);
    program = 0;

    material = nullptr;

    if (vertex_shader.get())
    {
        vertex_shader->Release();
        vertex_shader.reset();
    }
    if (pixel_shader.get())
    {
        pixel_shader->Release();
        pixel_shader.reset();
    }
    uniforms.clear();
}

void ShaderObjectOpenGL::Use() const
{
    glUseProgram(program);
}

void ShaderObjectOpenGL::BindMaterial(const Material* material)
{
    this->material = static_cast<const MaterialOpenGL*>(material);
    Set("material.albedo_text", static_cast<const Texture&>(this->material->GetAlbedoTexture()));
}

uint32_t ShaderObjectOpenGL::find_uniform_location(std::string_view uniform_name) const
{
    if (uniform_name.empty())
        return -1;

    auto it = uniforms.find(uniform_name.data());
    return (it != uniforms.end()) ? it->second : -1;
}

bool ShaderObjectOpenGL::AddVar(std::string_view uniform_name, uint32_t id)
{
    GLint location = find_uniform_location(uniform_name);
    if (location != -1)
        return true;

    uniforms.emplace(uniform_name.data(), id);
    return true;
}

bool ShaderObjectOpenGL::set_vec3f_impl(std::string_view uniform_name, const glm::vec3& vec)
{
    GLint location = find_uniform_location(uniform_name);
    if (location == -1)
        return false;

    glUniform3f(location, vec.x, vec.y, vec.z);
    return true;
}

bool ShaderObjectOpenGL::set_mat4f_impl(std::string_view uniform_name, const glm::mat4& matrix)
{
    GLint location = find_uniform_location(uniform_name);
    if (location == -1)
        return false;

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    return true;
}

bool ShaderObjectOpenGL::set_text2d_impl(std::string_view uniform_name, const Texture& texture)
{
    GLint location = find_uniform_location(uniform_name);
    if (location == -1)
        return false;

    const TextureOpenGL& text_gl = static_cast<const TextureOpenGL&>(texture);
    glUniform1i(location, text_gl.GetTextureUnit());
    glBindTextureUnit(text_gl.GetTextureUnit(), text_gl.GetTextureID());
    return true;
}

}  // namespace Fuego::Graphics
