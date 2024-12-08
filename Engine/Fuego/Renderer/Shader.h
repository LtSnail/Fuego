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

    static std::unique_ptr<Shader> CreateShader(const char* shaderCode, ShaderType type);
    virtual ~Shader() = default;

};
}  // namespace Fuego::Renderer
