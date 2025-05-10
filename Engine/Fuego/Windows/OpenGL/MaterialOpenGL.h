#pragma once

#include "Material.h"
#include "TextureOpenGL.h"

namespace Fuego::Graphics
{
class MaterialOpenGL : public Material
{
public:
    virtual ~MaterialOpenGL() override = default;

    inline const TextureOpenGL& GetAlbedoTexture() const
    {
        return *albedo_texture;
    }

    virtual void Use() const override;

private:
    const TextureOpenGL* albedo_texture;
    friend class Material;
    MaterialOpenGL(const Texture* albedo);
};
}  // namespace Fuego::Graphics
