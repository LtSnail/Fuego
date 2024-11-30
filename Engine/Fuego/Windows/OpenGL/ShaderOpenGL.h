#pragma once

#include "Renderer/Shader.h"

class ShaderOpenGL : public Fuego::Shader
{
    ShaderOpenGL(const std::string& vertexSrc, const std::string& fragmentSrc);

    virtual ~ShaderOpenGL();

    virtual void Bind();
};
