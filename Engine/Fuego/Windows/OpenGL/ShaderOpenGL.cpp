#include "ShaderOpenGL.h"

#include "ShaderObjectOpenGL.h"
#include "TextureOpenGL.h"
#include "glad/gl.h"

namespace Fuego::Graphics
{

GLint GetShaderType(Shader::ShaderType type)
{
    switch (type)
    {
    case Shader::Pixel:
        return GL_FRAGMENT_SHADER;
    case Shader::Vertex:
        return GL_VERTEX_SHADER;
    default:
        FU_CORE_ASSERT(false, "[Shader] Invalid shader type:")
        return 0;
    }
}

ShaderOpenGL::ShaderOpenGL(const char* shaderCode, ShaderType type)
    : shader_object(0)
    , _type(type)
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
    Release();
}

void ShaderOpenGL::BindToShaderObject(ShaderObject& obj)
{
    ShaderObjectOpenGL& obj_gl = dynamic_cast<ShaderObjectOpenGL&>(obj);
    shader_object = obj_gl.GetObjectID();
}

void ShaderOpenGL::Release()
{
    glDeleteShader(_shaderID);

    shader_object = 0;
    _shaderID = 0;
    _type = None;
}

}  // namespace Fuego::Graphics
