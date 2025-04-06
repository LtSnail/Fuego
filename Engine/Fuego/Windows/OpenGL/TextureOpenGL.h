#pragma once

#include "Renderer/Texture.h"

namespace Fuego::Renderer
{
class TextureOpenGL final : public Texture
{
public:
    TextureOpenGL() = default;
    ~TextureOpenGL() = default;

    virtual TextureFormat GetTextureFormat() const override;
    inline uint16_t GetTextureUnit() const
    {
        return texture_unit;
    }
    inline uint32_t GetTextureID() const
    {
        return texture_id;
    }

private:
    uint16_t texture_unit;
    uint32_t texture_id;
    friend class DeviceOpenGL;

    TextureOpenGL(unsigned char* buffer, int width, int heigth);
};

class TextureViewOpenGL
{
public:
    TextureViewOpenGL() = default;
    ~TextureViewOpenGL() = default;
};
}  // namespace Fuego::Renderer
