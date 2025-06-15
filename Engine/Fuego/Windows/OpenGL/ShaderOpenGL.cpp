#include "ShaderOpenGL.h"

#include <glm/gtc/type_ptr.hpp>

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

bool ShaderOpenGL::AddVar(const std::string& uniform)
{
    GLint location = glGetUniformLocation(shader_object, uniform.c_str());
    if (location != -1)
    {
        uniforms[uniform] = location;
        return true;
    }
    else
    {
        FU_CORE_ERROR("[Shader] Uniform {} not found in shader", uniform.c_str());
        return false;
    }
}

bool ShaderOpenGL::SetVec3f(const std::string& var, glm::vec3 vector) const
{
    if (auto it = uniforms.find(var); it != uniforms.end())
    {
        glUniform3f(it->second, vector.x, vector.y, vector.z);
        return true;
    }
    else
    {
        return false;
    }
}

bool ShaderOpenGL::SetMat4f(const std::string& var, glm::mat4 matrix) const
{
    if (auto it = uniforms.find(var); it != uniforms.end())
    {
        glUniformMatrix4fv(it->second, 1, GL_FALSE, glm::value_ptr(matrix));

        return true;
    }
    else
    {
        return false;
    }
}

bool ShaderOpenGL::SetText2D(const std::string& var, const Texture& texture) const
{
    const TextureOpenGL& text_gl = dynamic_cast<const TextureOpenGL&>(texture);
    if (auto it = uniforms.find(var); it != uniforms.end())
    {
        glUniform1i(it->second, text_gl.GetTextureUnit());
        glBindTextureUnit(text_gl.GetTextureUnit(), text_gl.GetTextureID());

        return true;
    }
    else
    {
        return false;
    }
}

void ShaderOpenGL::Release()
{
    glDeleteShader(_shaderID);

    shader_object = 0;
    _shaderID = 0;
    _type = None;
    uniforms.clear();
}

}  // namespace Fuego::Graphics
