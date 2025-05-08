#pragma once
#include <typeindex>
// #include <typeinfo>
#include <unordered_map>
#include <variant>

#include "FileSystem/FileSystem.h"
#include "Renderer.h"
#include "Services/ServiceInterfaces.hpp"
#include "singleton.hpp"

namespace Fuego
{

#pragma region Templates
using service_variant = std::variant<std::unique_ptr<Fuego::Graphics::Renderer>, std::unique_ptr<Fuego::FS::FileSystem>>;


template <class T>
concept is_renderer_service = std::derived_from<T, IRendererService>;

template <class T>
concept is_file_system_service = std::derived_from<T, IFileSystemService>;

template <class T>
concept is_one_of = is_renderer_service<T> || is_file_system_service<T>;

#pragma endregion

class ServiceLocator : public singleton<ServiceLocator>
{
    friend class singleton<ServiceLocator>;

public:
    template <is_one_of T>
    std::optional<T*> AddService()
    {
        auto ptr = std::make_unique<T>();
        service_variant variant = std::move(ptr);
        auto [it, inserted] = services.try_emplace(std::type_index(typeid(T)), std::move(variant));
        if (auto pval = std::get_if<std::unique_ptr<T>>(&it->second))
        {
            return pval->get();
        }
        return std::nullopt;
    };

    template <class T>
    T* GetService()
    {
        auto it = services.find(std::type_index(typeid(T)));
        if (it == services.end())
            return nullptr;

        if (auto ptr = std::get_if<std::unique_ptr<T>>(&it->second))
            return ptr->get();

        return nullptr;
    }

private:
    ServiceLocator() = default;
    std::unordered_map<std::type_index, service_variant> services;
};
}  // namespace Fuego