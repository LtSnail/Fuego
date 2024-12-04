#include "ShaderOpenGLFragment.h"

namespace Fuego::Renderer
{
ShaderOpenGLFragment::ShaderOpenGLFragment(const char* shader)
{
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, shader);

    _id = glCreateProgram();
    glAttachShader(_id, shader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    GLint success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(fragmentShader);
}
}  // namespace Fuego::Renderer
