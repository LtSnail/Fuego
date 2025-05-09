#pragma once
#include <glm/ext/matrix_float4x4.hpp>

#pragma region Concepts

template <typename Derived>
concept is_service_interface = requires(Derived t) {
    { t.Init() } -> std::same_as<void>;
    { t.Shutdown() } -> std::same_as<void>;
};

#pragma endregion

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
    void DrawModel(const Fuego::Graphics::Model* model, glm::mat4 model_pos);
    void ChangeViewport(float x, float y, float w, float h);
    std::unique_ptr<Fuego::Graphics::Texture> CreateTexture(unsigned char* buffer, int width, int height) const;
};

struct IFileSystemService
{
    virtual void FUCreateFile(const std::string& file_name, std::string_view folder) const = 0;
    virtual void WriteToFile(std::string_view file_name, const char* buffer) = 0;
};

struct IUpdatable
{
    void Update(float dlTime);
    void PostUpdate(float dlTime);
    void FixedUpdate();
};

struct IInitializable
{
    void Init();
    void Shutdown();
};

template <class Derived, class... Interfaces>
struct Service : public IInitializable, public Interfaces...
{
    void Init()
    {
        if (is_initialized)
            return;
        Derived* derived = static_cast<Derived*>(this);
        derived->OnInit();
        is_initialized = true;
    };

    void Shutdown()
    {
        if (shutdown)
            return;
        Derived* derived = static_cast<Derived*>(this);
        derived->OnShutdown();
        shutdown = true;
    }

protected:
    inline static bool is_initialized = false;
    inline static bool shutdown = false;
};

}  // namespace Fuego