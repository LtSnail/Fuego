#pragma once

namespace Fuego::Renderer
{
class Shader
{
public:
    enum ShaderType
    {
        Vertex = 1,
        Pixel = 2
    };

    virtual ~Shader() = default;
};
}  // namespace Fuego::Renderer
