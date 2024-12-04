#include "ShaderOpenGL.h"

namespace Fuego::Renderer
{
ShaderOpenGL::ShaderOpenGL()
    : _programID(0)
    , _vertexID(0)
    , _pixelID(0)
{
    _programID = glCreateProgram();
}

uint32_t ShaderOpenGL::CompileShader(GLenum shaderType, const char* shaderCode)
{
    uint32_t shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        FU_CORE_ERROR("[Shader] compilation error: ", infoLog);
    }

    return shader;
}

void ShaderOpenGL::Use(uint32_t ID) const
{
    glUseProgram(ID);
}

void ShaderOpenGL::BindPixelShader(const char* shaderCode)
{
    _pixelID = CompileShader(GL_FRAGMENT_SHADER, shaderCode);
    glAttachShader(_programID, _pixelID);

    GLint success;
    glGetProgramiv(_programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(_programID, 512, nullptr, infoLog);
        FU_CORE_ERROR("[Shader] linking error: ", infoLog);
    }

    glDeleteShader(_pixelID);
}

void ShaderOpenGL::BindVertexShader(const char* shaderCode)
{
    _vertexID = CompileShader(GL_VERTEX_SHADER, shaderCode);
    glAttachShader(_programID, _vertexID);

    GLint success;
    glGetProgramiv(_programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(_programID, 512, nullptr, infoLog);
        FU_CORE_ERROR("[Shader] linking error: ", infoLog);
    }

    glDeleteShader(_vertexID);
}

}  // namespace Fuego::Renderer
