#include "ShaderOpenGL.h"

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
}

ShaderOpenGL::~ShaderOpenGL()
{
    glDeleteShader(_shaderID);
}

GLint ShaderOpenGL::GetShaderType(ShaderType type) const
{
    switch (type)
    {
    case Pixel:
        return GL_FRAGMENT_SHADER;
    case Vertex:
        return GL_VERTEX_SHADER;
    }
}

}  // namespace Fuego::Renderer
