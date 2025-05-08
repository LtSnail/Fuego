#pragma once
#include <glm/ext/matrix_float4x4.hpp>

namespace Fuego::Graphics
{
class Model;
class Texture;
class Renderer;
}  // namespace Fuego::Graphics

namespace Fuego
{
struct IRendererService
{
    virtual void DrawModel(const Fuego::Graphics::Model* model, glm::mat4 model_pos) = 0;
    virtual void ChangeViewport(float x, float y, float w, float h) = 0;
    virtual std::unique_ptr<Fuego::Graphics::Texture> CreateTexture(unsigned char* buffer, int width, int height) const = 0;
};

struct IFileSystemService
{
    virtual void FUCreateFile(const std::string& file_name, std::string_view folder) const = 0;
    virtual void WriteToFile(std::string_view file_name, const char* buffer) = 0;
};

struct IEngineSubSystem
{
    virtual void Update(float dlTime) = 0;
    virtual void PostUpdate(float dlTime) = 0;
    virtual bool Init() = 0;
    virtual void Release() = 0;
};
}  // namespace Fuego