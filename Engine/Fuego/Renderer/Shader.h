#pragma once

namespace Fuego::Renderer
{
class Shader
{
public:
    enum ShaderType
    {
        None = 0,
        Vertex = 1,
        Pixel = 2
    };

    virtual ~Shader() = default;

    static std::unique_ptr<Shader> CreateShader(const char* shaderCode, ShaderType type);
};
}  // namespace Fuego::Renderer
