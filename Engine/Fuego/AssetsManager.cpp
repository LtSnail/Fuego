#include "AssetsManager.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "External/stb_image/stb_image.h"
#include "FileSystem/FileSystem.h"
#include "Services/ServiceLocator.h"

#define ASSIMP_LOAD_FLAGS aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType

Fuego::AssetsManager::AssetsManager(Fuego::Pipeline::Toolchain::assets_manager& toolchain)
    : models_count(0)
    , images2d_count(0)
    , toolchain(toolchain)
{
    models.reserve(10);
    images2d.reserve(10);
}

Fuego::AssetsManager::~AssetsManager()
{
    models.clear();
    images2d.clear();
}

// Models:
std::shared_ptr<Fuego::Graphics::Model> Fuego::AssetsManager::load_model(std::string_view path)
{
    if (path.empty())
        return std::shared_ptr<Fuego::Graphics::Model>();

    std::string file_name = std::filesystem::path(path.data()).stem().string();
    auto it = models.find(file_name);
    if (it != models.end())
        return it->second;

    auto fs = ServiceLocator::instance().GetService<Fuego::FS::FileSystem>();


    auto res = fs->GetFullPathToFile(path);
    if (!res)
        return std::shared_ptr<Fuego::Graphics::Model>{nullptr};

    Assimp::Importer importer{};
    const aiScene* scene = importer.ReadFile(res.value(), ASSIMP_LOAD_FLAGS);
    if (!scene)
        return std::shared_ptr<Fuego::Graphics::Model>{nullptr};

    std::shared_ptr<Fuego::Graphics::Model> model;
    {
        std::lock_guard<std::mutex> lock(models_async_operations);
        model = models.emplace(std::move(file_name), std::make_shared<Fuego::Graphics::Model>(scene)).first->second;
    }
    ++models_count;

    FU_CORE_INFO("[AssetsManager] Model[{0}] was added: name: {1}, ", models.size(), model->GetName());
    return model;
}
std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Model>> Fuego::AssetsManager::load_model_async(std::string_view path)
{
    if (path.empty())
        return std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Model>>{nullptr};

    std::string file_name = std::filesystem::path(path).stem().string();
    auto it = models.find(file_name);
    if (it != models.end())
        return std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Model>>{nullptr};

    auto handle = std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Model>>(std::make_shared<Fuego::Graphics::Model>(file_name), TO_BE_LOADED, NONE);

    auto thread_pool = ServiceLocator::instance().GetService<ThreadPool>();

    thread_pool->Submit(
        [this](std::string_view path, std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Model>> handle)
        {
            {
                std::lock_guard<std::mutex> lock(std::mutex());
                handle->SetStatus(LOADING);
            }

            auto fs = ServiceLocator::instance().GetService<Fuego::FS::FileSystem>();


            auto res = fs->GetFullPathToFile(path);
            if (!res)
            {
                std::lock_guard<std::mutex> lock(std::mutex());
                handle->SetStatus(CORRUPTED);
                handle->SetFailureReason(WRONG_PATH);
                return;
            }

            Assimp::Importer importer{};
            const aiScene* scene = importer.ReadFile(res.value(), ASSIMP_LOAD_FLAGS);
            if (!scene)
            {
                std::lock_guard<std::mutex> lock(std::mutex());
                handle->SetStatus(CORRUPTED);
                handle->SetFailureReason(NO_DATA);
                return;
            }

            {
                std::lock_guard<std::mutex> lock(std::mutex());
                handle->Resource()->PostLoad(scene);
                handle->SetStatus(SUCCESS);
                models.emplace(handle->Resource()->GetName(), handle->Resource());
            }
        },
        path, handle);

    return handle;
}

// Image:
std::shared_ptr<Fuego::Graphics::Image2D> Fuego::AssetsManager::load_image2d(std::string_view path)
{
    if (path.empty())
        return std::shared_ptr<Fuego::Graphics::Image2D>();

    std::string file_name = std::filesystem::path(path).stem().string();
    std::string ext = std::filesystem::path(path).extension().string();
    {
        std::lock_guard<std::mutex> lock(images2d_async_operations);
        auto image = images2d.find(file_name);
        if (image != images2d.end())
            return image->second;
    }

    auto fs = ServiceLocator::instance().GetService<Fuego::FS::FileSystem>();

    auto res = fs->GetFullPathToFile(path);
    if (!res)
        return std::shared_ptr<Fuego::Graphics::Image2D>{nullptr};

    uint16_t channels = ImageChannels(ext);
    stbi_set_flip_vertically_on_load(1);
    int w, h, bpp = 0;
    unsigned char* data = stbi_load(res.value().c_str(), &w, &h, &bpp, channels);
    if (!data)
    {
        FU_CORE_ERROR("Can't load an image: {0} {1}", path, stbi_failure_reason());
        return nullptr;
    }

    {
        std::lock_guard<std::mutex> lock(images2d_async_operations);
        auto img = images2d.emplace(file_name, std::make_shared<Fuego::Graphics::Image2D>(file_name, ext, data, w, h, bpp, channels)).first->second;
        FU_CORE_INFO("[AssetsManager] Image[{0}] was added: name: {1}, width: {2}, height: {3}", ++images2d_count, img->Name(), img->Width(), img->Height());
        return img;
    }
}
std::shared_ptr<Fuego::Graphics::Image2D> Fuego::AssetsManager::load_image2d_async(std::string_view path)
{
    if (path.empty())
        return std::shared_ptr<Fuego::Graphics::Image2D>{nullptr};

    std::string file_name = std::filesystem::path(path.data()).filename().stem().string();
    std::string ext = std::filesystem::path(path.data()).extension().string();
    std::shared_ptr<Fuego::Graphics::Image2D> placed_img;
    {
        std::lock_guard lock(images2d_async_operations);
        auto it = images2d.find(file_name);
        if (it != images2d.end())
            return it->second;

        placed_img = images2d.emplace(file_name, std::make_shared<Fuego::Graphics::Image2D>(file_name, std::move(ext))).first->second;
        ++images2d_count;
    }

    auto thread_pool = ServiceLocator::instance().GetService<ThreadPool>();

    thread_pool->Submit(
        [this](std::shared_ptr<Fuego::Graphics::Image2D> img)
        {
            auto fs = ServiceLocator::instance().GetService<Fuego::FS::FileSystem>();

            uint16_t channels = ImageChannels(img->Ext());

            stbi_set_flip_vertically_on_load(1);
            int w, h, bpp = 0;
            std::string full_name;
            full_name.reserve(img->Name().size() + img->Ext().size());
            full_name += img->Name();
            full_name += img->Ext();

            auto res = fs->GetFullPathToFile(full_name);
            // TODO: Mark as corrupted
            if (!res)
                return;

            unsigned char* data = stbi_load(res.value().c_str(), &w, &h, &bpp, channels);

            if (!data)
            {
                FU_CORE_ERROR("[Img2D]->AssetsManager::load_image2d_async, {0} - data is nullptr", full_name);
                return;
            }

            {
                std::lock_guard<std::mutex> guard(images2d_async_operations);

                Fuego::Graphics::Image2D::Image2DPostCreateion settings{static_cast<uint32_t>(w), static_cast<uint32_t>(h), static_cast<uint16_t>(bpp),
                                                                        channels, data};
                img->PostCreate(settings);
                FU_CORE_INFO("[AssetsManager] Image[{0}] was added: name: {1}, width: {2}, height: {3}", images2d.size(), img->Name(), img->Width(),
                             img->Height());
            }
        },
        placed_img);
    return placed_img;
}
std::shared_ptr<Fuego::Graphics::Image2D> Fuego::AssetsManager::LoadImage2DFromMemory(std::string_view name, unsigned char* data, uint32_t size_b,
                                                                                      uint16_t channels)
{
    if (!data)
        return std::shared_ptr<Fuego::Graphics::Image2D>(nullptr);

    std::string file_name = std::filesystem::path(name.data()).stem().string();
    std::string ext = std::filesystem::path(name.data()).extension().string();
    {
        std::lock_guard<std::mutex> lock(images2d_async_operations);
        auto image = images2d.find(file_name);
        if (image != images2d.end())
            return image->second;
    }

    int w, h, bpp = 0;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* img_data = stbi_load_from_memory(data, size_b, &w, &h, &bpp, channels);

    if (!img_data)
        return std::shared_ptr<Fuego::Graphics::Image2D>(nullptr);

    {
        std::lock_guard<std::mutex> lock(images2d_async_operations);
        auto img = images2d.emplace(file_name, std::make_shared<Fuego::Graphics::Image2D>(file_name, ext, img_data, w, h, bpp, channels)).first->second;
        FU_CORE_INFO("[AssetsManager] Image[{0}] was added: name: {1}, width: {2}, height: {3}", ++images2d_count, img->Name(), img->Width(), img->Height());
        return img;
    }
}
std::shared_ptr<Fuego::Graphics::Image2D> Fuego::AssetsManager::LoadImage2DFromMemoryAsync(std::string_view name, unsigned char* data, uint32_t size_b,
                                                                                           uint16_t channels)
{
    if (!data)
        return std::shared_ptr<Fuego::Graphics::Image2D>();

    std::string file_name = std::filesystem::path(name.data()).stem().string();
    std::string ext = std::filesystem::path(name.data()).extension().string();

    std::shared_ptr<Fuego::Graphics::Image2D> placed_img;
    {
        std::lock_guard lock(images2d_async_operations);
        auto image = images2d.find(file_name);
        if (image != images2d.end())
            return image->second;

        placed_img = images2d.emplace(file_name, std::make_shared<Fuego::Graphics::Image2D>(file_name, ext)).first->second;
        ++images2d_count;
    }

    auto thread_pool = ServiceLocator::instance().GetService<ThreadPool>();
    thread_pool->Submit(
        [this](std::shared_ptr<Fuego::Graphics::Image2D> img, unsigned char* data, uint32_t size_b, uint16_t channels)
        {
            if (!data)
                return;

            int w, h, bpp = 0;
            stbi_set_flip_vertically_on_load(1);
            unsigned char* img_data = stbi_load_from_memory(data, size_b, &w, &h, &bpp, channels);

            if (!img_data)
                return;

            {
                std::lock_guard<std::mutex> lock(images2d_async_operations);
                Fuego::Graphics::Image2D::Image2DPostCreateion settings{static_cast<uint32_t>(w), static_cast<uint32_t>(h), static_cast<uint16_t>(bpp),
                                                                        channels, img_data};
                img->PostCreate(settings);
            }
            FU_CORE_INFO("[AssetsManager] Image was added: name: {0}, width: {1}, height: {2}", img->Name(), img->Width(), img->Height());
        },
        placed_img, data, size_b, channels);
    return placed_img;
}
std::shared_ptr<Fuego::Graphics::Image2D> Fuego::AssetsManager::LoadImage2DFromRawData(std::string_view name, unsigned char* data, uint32_t channels,
                                                                                       uint16_t bpp, uint32_t width, uint32_t height)
{
    if (!data || name.empty())
        return std::shared_ptr<Fuego::Graphics::Image2D>();

    {
        std::string file_name = std::filesystem::path(name.data()).stem().string();
        std::string ext = std::filesystem::path(name.data()).extension().string();

        std::lock_guard lock(images2d_async_operations);
        auto image = images2d.find(file_name);
        if (image != images2d.end())
            return image->second;

        auto img = images2d.emplace(std::move(file_name), std::make_shared<Fuego::Graphics::Image2D>(file_name, ext, data, width, height, bpp, channels))
                       .first->second;
        FU_CORE_INFO("[AssetsManager] Image[{0}] was added: name: {1}, width: {2}, height: {3}", ++images2d_count, img->Name(), img->Width(), img->Height());
        return img;
    }
}
uint16_t Fuego::AssetsManager::ImageChannels(std::string_view image2d_ext)
{
    if (image2d_ext.empty() || image2d_ext.size() > 3)
        return static_cast<uint16_t>(3);

    if (image2d_ext.compare("jpg"))
    {
        return static_cast<uint16_t>(3);
    }
    else if (image2d_ext.compare("png"))
    {
        return static_cast<uint16_t>(4);
    }
    else
    {
        return static_cast<uint16_t>(3);
    }
}
