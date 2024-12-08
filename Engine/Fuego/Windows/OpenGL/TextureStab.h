#pragma once

#include "Renderer/Texture.h"

namespace Fuego::Renderer
{
class TextureStab : public Texture
{
public:
    TextureStab() = default;
    virtual ~TextureStab() override;

    virtual TextureFormat GetTextureFormat() const override;
};

class TextureViewStab
{
public:
    TextureViewStab() = default;
    virtual ~TextureViewStab() = default;
};
}