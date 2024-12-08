#include "ShaderOpenGL.h"

namespace Fuego::Renderer
{
ShaderOpenGL::ShaderOpenGL(const char* shaderCode, ShaderType type)
    : _shaderID(0)
    , _type(ShaderType::None)
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

std::unique_ptr<Shader> Shader::CreateShader(const char* shaderCode, ShaderType type){return Shader::CreateShader(shaderCode, type);}

ShaderOpenGL::~ShaderOpenGL()
{
    glDeleteShader(_shaderID);
}

GLint ShaderOpenGL::GetShaderType(ShaderType type) const
{
    switch (type)
    {
    case ShaderType::Pixel:
        return GL_FRAGMENT_SHADER;
    case ShaderType::Vertex:
        return GL_FRAGMENT_SHADER;

    case ShaderType::None:
    default:
        return 0;
    }
}

}  // namespace Fuego::Renderer
