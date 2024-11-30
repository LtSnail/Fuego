#include "ShaderOpenGL.h"

#include "glad/glad.h"

ShaderOpenGL::ShaderOpenGL(const std::string& vertexSrc, const std::string& fragmentSrc)
    : _shaderID(0)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* source = (const GLchar*)vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        FU_CORE_ERROR("{0}", infoLog.data());

        glDeleteShader(vertexShader);
        return;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    source = (const GLchar*)fragmentSrc.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        FU_CORE_ERROR("{0}", infoLog.data());
        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        return;
    }

    _shaderID = glCreateProgram();
    GLuint program = _shaderID;
    // Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        FU_CORE_ERROR("{0}", infoLog.data());
        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return;
    }
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
}

ShaderOpenGL::~ShaderOpenGL()
{
    glUseProgram(_shaderID);
}

void ShaderOpenGL::Bind() const
{
    glUseProgram(_shaderID);
}
