#pragma once

#include "Renderer/Shader.h"

class ShaderOpenGL : public Fuego::Shader
{
public:
    ShaderOpenGL(const std::string& vertexSrc, const std::string& fragmentSrc);

    virtual ~ShaderOpenGL();

    virtual void Bind() const override;

private:
    uint32_t _shaderID;
};
