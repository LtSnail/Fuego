#pragma once

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

struct IUpdatable
{
    void OnUpdate(float dlTime);
    void OnPostUpdate(float dlTime);
    void OnFixedUpdate();
};

struct IInitializable
{
    void Init();
    void Shutdown();
};

template <class Derived>
struct Service : public IInitializable
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