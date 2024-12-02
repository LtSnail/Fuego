#pragma once

namespace Fuego::Renderer
{
class Texture
{
public:
    virtual ~Texture() = default;
};

class TextureView
{
public:
    virtual ~TextureView() = default;
};
}  // namespace Fuego::Renderer
