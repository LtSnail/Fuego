#pragma once

#include "Renderer/Texture.h"

namespace Fuego::Renderer
{
class TextureOpenGL : public Texture
{
public:
    TextureOpenGL() = default;
    ~TextureOpenGL() = default;

    virtual TextureFormat GetTextureFormat() const override;
};

class TextureViewOpenGL
{
public:
    TextureViewOpenGL() = default;
    ~TextureViewOpenGL() = default;
};
}  // namespace Fuego::Renderer
