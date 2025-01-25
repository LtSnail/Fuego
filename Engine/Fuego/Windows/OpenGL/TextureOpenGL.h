#pragma once

#include "Renderer/Texture.h"

namespace Fuego::Renderer
{
class TextureOpenGL final : public Texture
{
public:
    TextureOpenGL() = default;

    virtual TextureFormat GetTextureFormat() const override;
};

class TextureViewOpenGL
{
public:
    TextureViewOpenGL() = default;
    ~TextureViewOpenGL() = default;
};
}  // namespace Fuego::Renderer
