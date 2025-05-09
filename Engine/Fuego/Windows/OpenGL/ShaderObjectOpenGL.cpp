#include "ShaderObjectOpenGL.h"

#include "glad/gl.h"

namespace Fuego::Graphics
{

ShaderObject* ShaderObject::CreateShaderObject(Shader& vs, Shader& px)
{
    return new ShaderObjectOpenGL(vs, px);
}

ShaderObjectOpenGL::ShaderObjectOpenGL(Shader& vs, Shader& px)
    : program(glCreateProgram())
    , vertex_shader(nullptr)
    , pixel_shader(nullptr)
    , material(nullptr)
{
    auto vs_gl = dynamic_cast<ShaderOpenGL*>(&vs);
    auto px_gl = dynamic_cast<ShaderOpenGL*>(&px);
    vertex_shader = vs_gl;
    pixel_shader = px_gl;

    glAttachShader(program, vs_gl->GetID());
    glAttachShader(program, px_gl->GetID());

    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        FU_CORE_ERROR("[ShaderObject] program linking error: ", infoLog);
    }
    vs_gl->BindToShaderObject(*this);
    px_gl->BindToShaderObject(*this);

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

void ShaderObjectOpenGL::BindMaterial(Material* material)
{
    this->material = static_cast<MaterialOpenGL*>(material);
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
