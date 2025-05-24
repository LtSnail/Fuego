#pragma once

#include <filesystem>
#include <type_traits>

#include "Services/ServiceInterfaces.hpp"

namespace Fuego::Graphics
{
class Image2D;
class Model;
enum ImageFormat;
}  // namespace Fuego::Graphics

namespace Fuego
{


class AssetsManager : public Service<AssetsManager>
{
public:
    friend class Service<AssetsManager>;

    AssetsManager();
    ~AssetsManager();

    template <class Res>
    std::shared_ptr<Res> Load(std::string_view path)
    {
        if constexpr (std::is_same<std::remove_cv_t<Res>, std::remove_cv_t<Fuego::Graphics::Model>>::value)
        {
            auto model = load_model(path);
            FU_CORE_ASSERT(model, "[Assets manager] cannot load model")
            return model;
        }
        else if constexpr (std::is_same<std::remove_cv_t<Res>, std::remove_cv_t<Fuego::Graphics::Image2D>>::value)
        {
            auto img = load_image2d(path);
            FU_CORE_ASSERT(img, "[Assets manager] cannot load image2d");
            return img;
        }
        else
        {
            FU_CORE_ASSERT(nullptr, "[Assets manager] wront graphics resource type")
        }
    }

    std::shared_ptr<Fuego::Graphics::Image2D> LoadImage2DFromMemory(std::string_view name, unsigned char* data, uint32_t size_b, uint16_t channels);

    std::shared_ptr<Fuego::Graphics::Image2D> LoadImage2DFromRawData(std::string_view name, unsigned char* data, uint32_t channels, uint16_t bpp,
                                                                     uint32_t width, uint32_t height);

    template <class Res>
    void LoadAsync(std::string_view path)
    {
        if constexpr (std::is_same<std::remove_cv_t<Res>, std::remove_cv_t<Fuego::Graphics::Image2D>>::value)
        {
            load_image2d_async(path);
        }
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
                models.erase(it);
                --models_count;
            }
        }
        else if constexpr (std::is_same<std::remove_cv_t<Res>, std::remove_cv_t<Fuego::Graphics::Image2D>>::value)
        {
            auto it = images2d.find(name);
            if (it != images2d.end())
            {
                FU_CORE_INFO("[Assets manager] Image2D: {0} has been erased", it->first);
                images2d.erase(it);
                --images2d_count;
            }
        }
        else
        {
            FU_CORE_ASSERT(nullptr, "[Assets manager] wront graphics resource type")
        }
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Fuego::Graphics::Model>> models;
    std::unordered_map<std::string, std::shared_ptr<Fuego::Graphics::Image2D>> images2d;

    std::shared_ptr<Fuego::Graphics::Model> load_model(std::string_view path);
    void load_model_async(std::string_view path);
    std::shared_ptr<Fuego::Graphics::Image2D> load_image2d(std::string_view path);
    void load_image2d_async(std::string_view path);

    std::atomic<uint32_t> models_count;
    std::atomic<uint32_t> images2d_count;

    std::mutex models_async_operations;
    std::mutex images2d_async_operations;

    uint16_t ImageChannels(std::string_view image2d_ext);
};


}  // namespace Fuego