#include "MaterialOpenGL.h"

#include "ShaderObjectOpenGL.h"
#include <glad/gl.h>

namespace Fuego::Graphics
{

Material* Material::CreateMaterial(const Texture* albedo)
{
    return new MaterialOpenGL(albedo);
}

MaterialOpenGL::MaterialOpenGL(const Texture* albedo)
    : albedo_texture(nullptr)
{
    albedo_texture = static_cast<const TextureOpenGL*>(albedo);
}

void MaterialOpenGL::Use() const
{
    glBindTexture(GL_TEXTURE_2D, albedo_texture->GetTextureID());
}

}  // namespace Fuego::Graphics
