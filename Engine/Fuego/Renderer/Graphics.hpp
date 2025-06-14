#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Fuego::Graphics
{

enum GraphicsAPI
{
    OpenGL = 0,
    Vulkan = 1,
};

#pragma pack(push, 1)
struct VertexData
{
    glm::vec3 pos;
    glm::vec2 textcoord;
    glm::vec3 normal;

    VertexData(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 text_coord = glm::vec3(0.0f),
               glm::vec3 normal = glm::vec3(0.0f));
};
#pragma pack(pop)

struct Viewport
{
    float width = 0.0f;
    float height = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
};

enum TextureType
{
    ALBEDO = 0,
    DIFFUSE = 1,
    SPECULAR = 2
};

enum RenderStage
{
    STATIC_GEOMETRY,
    DYNAMIC_DRAW
};

}  // namespace Fuego::Graphics
