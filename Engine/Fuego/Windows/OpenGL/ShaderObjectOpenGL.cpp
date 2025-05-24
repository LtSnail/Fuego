#include "ShaderObjectOpenGL.h"

#include "glad/gl.h"

namespace Fuego::Graphics
{

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
    vertex_shader->BindToShaderObject(*this);
    pixel_shader->BindToShaderObject(*this);

    glUseProgram(0);
}

ShaderObjectOpenGL::~ShaderObjectOpenGL()
{
    Release();
}

void ShaderObjectOpenGL::Use() const
{
    glUseProgram(program);
}

void ShaderObjectOpenGL::BindMaterial(const Material* material)
{
    this->material = static_cast<const MaterialOpenGL*>(material);
    pixel_shader->AddVar("material.albedo_text");
    pixel_shader->SetText2D("material.albedo_text", this->material->GetAlbedoTexture());
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
}

}  // namespace Fuego::Graphics
