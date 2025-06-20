#pragma once

namespace Fuego::Graphics
{
class ShaderObject;
class Texture;

class Shader
{
public:
    enum ShaderType
    {
        None = 0,
        Vertex = 1,
        Pixel = 2
    };

    friend class ShaderObject;
    virtual void BindToShaderObject(ShaderObject& obj) = 0;

    virtual ~Shader() = default;

    virtual void Release() = 0;
};
}  // namespace Fuego::Graphics
