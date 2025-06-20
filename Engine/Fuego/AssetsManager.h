#pragma once

#include <filesystem>
#include <type_traits>

#include "ApplicationPipeline.hpp"
#include "Renderer/Color.h"
#include "Services/ServiceInterfaces.hpp"
#include "tbb/concurrent_unordered_map.h"

namespace Fuego::Graphics
{
class Image2D;
class Model;
enum ImageFormat;
}  // namespace Fuego::Graphics

namespace Fuego
{
enum Failure
{
    WRONG_PATH,
    NO_DATA
};

enum LoadingSts
{
    TO_BE_LOADED,
    LOADING,
    CORRUPTED,
    SUCCESS
};

template <typename T>
class ResourceHandle
{
public:
    ResourceHandle(std::shared_ptr<T> resource, LoadingSts status, std::optional<Failure> failure = std::nullopt)
        : obj(resource)
        , status(status)
        , failure(failure) {};

    ResourceHandle(std::shared_ptr<T> resource)
        : obj(resource)
        , status(status) {};

    ~ResourceHandle() = default;

    LoadingSts Status()
    {
        return status;
    }
    std::optional<Failure> FailureReason()
    {
        return failure;
    }
    void SetCorrupted(Failure failure)
    {
        status = CORRUPTED;
        failure = failure;
    }
    void SetSuccess()
    {
        status = SUCCESS;
        failure = std::nullopt;
    }
    void SetStatus(LoadingSts st)
    {
        status = st;
    }
    void SetFailure(Failure fail)
    {
        failure = fail;
    }
    std::shared_ptr<T> Resource()
    {
        return obj;
    }

private:
    std::shared_ptr<T> obj;
    LoadingSts status{TO_BE_LOADED};
    std::optional<Failure> failure;
};

class AssetsManager : public Service<AssetsManager>
{
public:
    friend class Application;
    friend class Service<AssetsManager>;

    AssetsManager(Fuego::Pipeline::Toolchain::assets_manager& toolchain);
    ~AssetsManager();

    std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> LoadImage2DFromMemory(std::string_view name, unsigned char* data, uint32_t size_b,
                                                                                           uint16_t channels);
    std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> LoadImage2DFromMemoryAsync(std::string_view name, unsigned char* data, uint32_t size_b,
                                                                                                uint16_t channels);

    std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> LoadImage2DFromRawData(std::string_view name, unsigned char* data, uint32_t channels,
                                                                                            uint16_t bpp, uint32_t width, uint32_t height);

    std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> LoadImage2DFromColor(std::string_view name, Fuego::Graphics::Color color, uint32_t width,
                                                                                          uint32_t height);

    template <class Res>
    std::shared_ptr<Fuego::ResourceHandle<Res>> Load(std::string_view path, bool async = true)
    {
        std::shared_ptr<Fuego::ResourceHandle<Res>> result{nullptr};
        if constexpr (std::is_same_v<std::remove_cv_t<Res>, Fuego::Graphics::Image2D>)
        {
            if (async)
                return load_image2d_async(path);
            else
                return load_image2d(path);
        }
        else if constexpr (std::is_same_v<std::remove_cv_t<Res>, Fuego::Graphics::Model>)
        {
            if (async)
                return load_model_async(path);
            else
                return load_model(path);
        }
        FU_CORE_ASSERT(false, "");
        return std::shared_ptr<Fuego::ResourceHandle<Res>>{};
    }

    template <class Res>
    std::weak_ptr<Res> Get(std::string_view name)
    {
        if (name.empty())
            return std::weak_ptr<Res>{};

        if constexpr (std::is_same<std::remove_cv_t<Res>, std::remove_cv_t<Fuego::Graphics::Model>>::value)
        {
            auto it = models.find(name.data());
            if (it != models.end())
                return std::weak_ptr<Res>(it->second);
            else
                return std::weak_ptr<Res>{};
        }
        else if constexpr (std::is_same<std::remove_cv_t<Res>, std::remove_cv_t<Fuego::Graphics::Image2D>>::value)
        {
            auto it = images2d.find(name.data());
            if (it != images2d.end())
                return std::weak_ptr<Res>(it->second);
            else
                return std::weak_ptr<Res>{};
        }
        else
            FU_CORE_ASSERT(nullptr, "[Assets manager] wront graphics resource type")
    }

    template <class Res>
    void Unload(std::string_view res_name)
    {
        if (res_name.empty())
            return;

        std::string name = std::filesystem::path(res_name).stem().string();

        if constexpr (std::is_same<std::remove_cv_t<Res>, std::remove_cv_t<Fuego::Graphics::Model>>::value)
        {
            auto it = models.find(name);
            if (it != models.end())
            {
                FU_CORE_INFO("[Assets manager] Model: {0} has been erased", it->first);
                std::mutex mtx;
                std::lock_guard<std::mutex> lock(mtx);
                models.unsafe_erase(it);
            }
            --models_count;
        }
        else if constexpr (std::is_same<std::remove_cv_t<Res>, std::remove_cv_t<Fuego::Graphics::Image2D>>::value)
        {
            auto it = images2d.find(name);
            if (it != images2d.end())
            {
                FU_CORE_INFO("[Assets manager] Image2D: {0} has been erased", it->first);
                std::mutex mtx;
                std::lock_guard<std::mutex> lock(mtx);
                images2d.unsafe_erase(it);
            }
            --images2d_count;
        }
        else
            FU_CORE_ASSERT(nullptr, "[Assets manager] wront graphics resource type");
    }

    void FreeImage2D(unsigned char* data) const;

private:
    tbb::concurrent_unordered_map<std::string, std::shared_ptr<Fuego::Graphics::Model>> models;

    //  TODO: What to do with corrupted models?
    tbb::concurrent_unordered_map<std::string, std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Model>>> models_to_load_async;

    tbb::concurrent_unordered_map<std::string, std::shared_ptr<Fuego::Graphics::Image2D>> images2d;
    tbb::concurrent_unordered_map<std::string, std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>> images2d_to_load_async;

    std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Model>> load_model(std::string_view path);
    std::shared_ptr<ResourceHandle<Fuego::Graphics::Model>> load_model_async(std::string_view path);

    std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> load_image2d(std::string_view path);
    std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> load_image2d_async(std::string_view path);

    std::atomic<uint32_t> models_count;
    std::atomic<uint32_t> images2d_count;

    uint16_t ImageChannels(std::string_view image2d_ext);

    Fuego::Pipeline::Toolchain::assets_manager toolchain;

    template <typename Map>
    bool is_already_loaded(const Map& map, const std::string& key, std::shared_ptr<ResourceHandle<typename Map::mapped_type::element_type>>& handle_out)
    {
        auto it = map.find(key);
        if (it != map.end())
        {
            handle_out = std::make_shared<ResourceHandle<typename Map::mapped_type::element_type>>(it->second);
            handle_out->SetSuccess();
            return true;
        }
        return false;
    }
};

}  // namespace Fuego
