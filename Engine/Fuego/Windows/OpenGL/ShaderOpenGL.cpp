#include "ShaderOpenGL.h"

#include "glad/gl.h"

namespace
{
GLint GetShaderType(Fuego::Renderer::ShaderOpenGL::ShaderType type)
{
    switch (type)
    {
    case Fuego::Renderer::Shader::Pixel:
        return GL_FRAGMENT_SHADER;
    case Fuego::Renderer::Shader::Vertex:
        return GL_VERTEX_SHADER;
    }

    FU_CORE_ERROR("[Shader] Invalid shader type:");
    return 0;
}
}  // namespace


namespace Fuego::Renderer
{
ShaderOpenGL::ShaderOpenGL(const char* shaderCode, ShaderType type)
    : _type(type)
{
    _shaderID = glCreateShader(GetShaderType(type));
    glShaderSource(_shaderID, 1, &shaderCode, nullptr);
    glCompileShader(_shaderID);

    GLint success;
    glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(_shaderID, 512, nullptr, infoLog);
        FU_CORE_ERROR("[Shader] compilation error: ", infoLog);
    }
    else
    {
        FU_CORE_TRACE("[Shader] has compiled");
    }
}

ShaderOpenGL::~ShaderOpenGL()
{
    glDeleteShader(_shaderID);
}
}  // namespace Fuego::Renderer
