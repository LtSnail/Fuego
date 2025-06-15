#include "AssetsManager.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "External/stb_image/stb_image.h"
#include "FileSystem/FileSystem.h"
#include "Services/ServiceLocator.h"

#define ASSIMP_LOAD_FLAGS \
    aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType

Fuego::AssetsManager::AssetsManager(Fuego::Pipeline::Toolchain::assets_manager& toolchain)
    : models_count(0), images2d_count(0), toolchain(toolchain)
{
    models.reserve(10);
    images2d.reserve(10);
}

Fuego::AssetsManager::~AssetsManager()
{
    models.clear();
    images2d.clear();
}

void Fuego::AssetsManager::FreeImage2D(unsigned char* data) const
{
    if (data)
        stbi_image_free(data);
}

// Models:
std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Model>> Fuego::AssetsManager::load_model(std::string_view path)
{
    if (path.empty())
        return std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Model>>{nullptr};

    std::string file_name = std::filesystem::path(path).stem().string();
    auto it = models.find(file_name);
    if (it != models.end())
        return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Model>>(
            it->second, ResourceLoadingStatus::SUCCESS, ResourceLoadingFailureReason::NONE);

    auto fs = ServiceLocator::instance().GetService<Fuego::FS::FileSystem>();

    auto handle = std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Model>>(
        std::make_shared<Fuego::Graphics::Model>(file_name), ResourceLoadingStatus::TO_BE_LOADED,
        ResourceLoadingFailureReason::NONE);

    auto res = fs->GetFullPathToFile(path);
    if (!res)
    {
        handle->SetStatus(ResourceLoadingStatus::CORRUPTED);
        handle->SetFailureReason(ResourceLoadingFailureReason::WRONG_PATH);
        return handle;
    }

    Assimp::Importer importer{};
    const aiScene* scene = importer.ReadFile(res.value(), ASSIMP_LOAD_FLAGS);
    if (!scene)
    {
        handle->SetStatus(ResourceLoadingStatus::CORRUPTED);
        handle->SetFailureReason(ResourceLoadingFailureReason::NO_DATA);
        return handle;
    }

    std::shared_ptr<Fuego::Graphics::Model> model;
    models.emplace(std::move(file_name), handle->Resource());
    ++models_count;

    FU_CORE_INFO("[AssetsManager] Model[{0}] was added: name: {1}, ", models.size(), model->GetName());
    return handle;
}
std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Model>> Fuego::AssetsManager::load_model_async(
    std::string_view path)
{
    if (path.empty())
        return std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Model>>{nullptr};

    std::string file_name = std::filesystem::path(path).stem().string();
    const auto it = models.find(file_name);
    if (it != models.end())
        return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Model>>(
            it->second, ResourceLoadingStatus::SUCCESS, ResourceLoadingFailureReason::NONE);
    {
        const auto it = models_to_load_async.find(file_name);
        if (it != models_to_load_async.end() && it->second->Status() != ResourceLoadingStatus::CORRUPTED)
            return it->second;
    }

    auto handle = models_to_load_async
                      .emplace(file_name, std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Model>>(
                                              std::make_shared<Fuego::Graphics::Model>(file_name),
                                              ResourceLoadingStatus::TO_BE_LOADED, ResourceLoadingFailureReason::NONE))
                      .first->second;

    auto thread_pool = ServiceLocator::instance().GetService<ThreadPool>();

    thread_pool->Submit(
        [this](std::string_view path, std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Model>> handle)
        {
            handle->SetStatus(ResourceLoadingStatus::LOADING);

            auto fs = ServiceLocator::instance().GetService<Fuego::FS::FileSystem>();

            auto res = fs->GetFullPathToFile(path);
            if (!res)
            {
                handle->SetStatus(ResourceLoadingStatus::CORRUPTED);
                handle->SetFailureReason(ResourceLoadingFailureReason::WRONG_PATH);
                return;
            }

            Assimp::Importer importer{};
            const aiScene* scene = importer.ReadFile(res.value(), ASSIMP_LOAD_FLAGS);
            if (!scene)
            {
                handle->SetStatus(ResourceLoadingStatus::CORRUPTED);
                handle->SetFailureReason(ResourceLoadingFailureReason::NO_DATA);
                return;
            }

            handle->Resource()->PostLoad(scene);
            handle->SetStatus(ResourceLoadingStatus::SUCCESS);
            auto model = models.emplace(handle->Resource()->GetName(), handle->Resource()).first->second;
            FU_CORE_INFO("[AssetsManager] Model[{0}] was added: name: {1}, ", models.size(), model->GetName());
            ++models_count;

            auto it = models_to_load_async.find(handle->Resource()->GetName().data());
            if (it != models_to_load_async.end())
            {
                std::mutex mtx;
                std::lock_guard<std::mutex> lock(mtx);
                models_to_load_async.unsafe_erase(it);
            }
        },
        path, handle);

    return handle;
}

// Image:
std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> Fuego::AssetsManager::load_image2d(
    std::string_view path)
{
    if (path.empty())
        return std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>{nullptr};

    std::string file_name = std::filesystem::path(path).stem().string();
    std::string ext = std::filesystem::path(path).extension().string();

    auto image = images2d.find(file_name);
    if (image != images2d.end())
        return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(
            image->second, ResourceLoadingStatus::SUCCESS, ResourceLoadingFailureReason::NONE);

    auto fs = ServiceLocator::instance().GetService<Fuego::FS::FileSystem>();

    auto res = fs->GetFullPathToFile(path);
    if (!res)
        return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(
            ResourceLoadingStatus::CORRUPTED, ResourceLoadingFailureReason::WRONG_PATH);

    uint16_t channels = ImageChannels(ext);
    stbi_set_flip_vertically_on_load(1);
    int w, h, bpp = 0;
    unsigned char* data = stbi_load(res.value().c_str(), &w, &h, &bpp, channels);
    if (!data)
    {
        FU_CORE_ERROR("Can't load an image: {0} {1}", path, stbi_failure_reason());
        return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(ResourceLoadingStatus::CORRUPTED,
                                                                                 ResourceLoadingFailureReason::NO_DATA);
    }

    auto img =
        images2d
            .emplace(file_name, std::make_shared<Fuego::Graphics::Image2D>(file_name, ext, data, w, h, bpp, channels))
            .first->second;
    FU_CORE_INFO("[AssetsManager] Image[{0}] was added: name: {1}, width: {2}, height: {3}", ++images2d_count,
                 img->Name(), img->Width(), img->Height());
    return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(img, ResourceLoadingStatus::SUCCESS,
                                                                             ResourceLoadingFailureReason::NONE);
}
std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> Fuego::AssetsManager::load_image2d_async(
    std::string_view path)
{  //
    if (path.empty())
        return std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>{nullptr};

    std::string file_name = std::filesystem::path(path).filename().stem().string();
    std::string ext = std::filesystem::path(path).extension().string();

    auto it = images2d.find(file_name);
    if (it != images2d.end())
        return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(
            it->second, ResourceLoadingStatus::SUCCESS, ResourceLoadingFailureReason::NONE);
    {
        auto it = images2d_to_load_async.find(file_name);
        if (it != images2d_to_load_async.end() && it->second->Status() != ResourceLoadingStatus::CORRUPTED)
            return it->second;
    }

    auto handle = images2d_to_load_async
                      .emplace(file_name, std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(
                                              std::make_shared<Fuego::Graphics::Image2D>(file_name, ext),
                                              ResourceLoadingStatus::TO_BE_LOADED, ResourceLoadingFailureReason::NONE))
                      .first->second;

    auto thread_pool = ServiceLocator::instance().GetService<ThreadPool>();

    thread_pool->Submit(
        [this](std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> handle)
        {
            auto fs = ServiceLocator::instance().GetService<Fuego::FS::FileSystem>();
            auto img = handle->Resource();
            uint16_t channels = ImageChannels(img->Ext());

            stbi_set_flip_vertically_on_load(1);
            int w, h, bpp = 0;

            std::filesystem::path full_path = img->Name();
            full_path.replace_extension(img->Ext());

            auto res = fs->GetFullPathToFile(full_path.string());
            if (!res)
            {
                handle->SetStatus(ResourceLoadingStatus::CORRUPTED);
                handle->SetFailureReason(ResourceLoadingFailureReason::WRONG_PATH);
                return;
            }

            unsigned char* data = stbi_load(res.value().c_str(), &w, &h, &bpp, channels);

            if (!data)
            {
                handle->SetStatus(ResourceLoadingStatus::CORRUPTED);
                handle->SetFailureReason(ResourceLoadingFailureReason::NO_DATA);
                return;
            }
            Fuego::Graphics::Image2D::Image2DPostCreateion settings{static_cast<uint32_t>(w), static_cast<uint32_t>(h),
                                                                    static_cast<uint16_t>(bpp), channels, data};
            handle->Resource()->PostCreate(settings);
            handle->SetStatus(ResourceLoadingStatus::SUCCESS);

            auto image = images2d.emplace(handle->Resource()->Name(), handle->Resource()).first->second;
            FU_CORE_INFO("[AssetsManager] Image was added: name: {0}, ", image->Name());
            ++images2d_count;

            auto it = images2d_to_load_async.find(handle->Resource()->Name().data());
            if (it != images2d_to_load_async.end())
            {
                std::mutex mtx;
                std::lock_guard<std::mutex> lock(mtx);
                images2d_to_load_async.unsafe_erase(it);
            }
        },
        handle);
    return handle;
}
std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> Fuego::AssetsManager::LoadImage2DFromMemory(
    std::string_view name, unsigned char* data, uint32_t size_b, uint16_t channels)
{
    if (!data)
        return std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>{nullptr};

    std::string file_name = std::filesystem::path(name.data()).stem().string();
    std::string ext = std::filesystem::path(name.data()).extension().string();
    auto image = images2d.find(file_name);
    if (image != images2d.end())
        return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(
            image->second, ResourceLoadingStatus::SUCCESS, ResourceLoadingFailureReason::NONE);

    int w, h, bpp = 0;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* img_data = stbi_load_from_memory(data, size_b, &w, &h, &bpp, channels);

    if (!img_data)
        return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(ResourceLoadingStatus::CORRUPTED,
                                                                                 ResourceLoadingFailureReason::NO_DATA);

    auto img = images2d
                   .emplace(file_name,
                            std::make_shared<Fuego::Graphics::Image2D>(file_name, ext, img_data, w, h, bpp, channels))
                   .first->second;
    FU_CORE_INFO("[AssetsManager] Image[{0}] was added: name: {1}, width: {2}, height: {3}", ++images2d_count,
                 img->Name(), img->Width(), img->Height());
    return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(img, ResourceLoadingStatus::SUCCESS,
                                                                             ResourceLoadingFailureReason::NONE);
}
std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> Fuego::AssetsManager::LoadImage2DFromMemoryAsync(
    std::string_view name, unsigned char* data, uint32_t size_b, uint16_t channels)
{
    if (!data)
        return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(ResourceLoadingStatus::CORRUPTED,
                                                                                 ResourceLoadingFailureReason::NO_DATA);

    std::string file_name = std::filesystem::path(name.data()).stem().string();
    std::string ext = std::filesystem::path(name.data()).extension().string();

    auto image = images2d.find(file_name);
    if (image != images2d.end())
        return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(
            image->second, ResourceLoadingStatus::SUCCESS, ResourceLoadingFailureReason::NONE);
    auto it = images2d_to_load_async.find(file_name);
    if (it != images2d_to_load_async.end() && it->second->Status() != ResourceLoadingStatus::CORRUPTED)
        return it->second;

    auto handle = images2d_to_load_async
                      .emplace(file_name, std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(
                                              std::make_shared<Fuego::Graphics::Image2D>(file_name, ext),
                                              ResourceLoadingStatus::TO_BE_LOADED, ResourceLoadingFailureReason::NONE))
                      .first->second;

    auto thread_pool = ServiceLocator::instance().GetService<ThreadPool>();
    thread_pool->Submit(
        [this](std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> handle, unsigned char* data,
               uint32_t size_b, uint16_t channels)
        {
            if (!data)
            {
                handle->SetStatus(ResourceLoadingStatus::CORRUPTED);
                handle->SetFailureReason(ResourceLoadingFailureReason::NO_DATA);
                return;
            }

            int w, h, bpp = 0;
            stbi_set_flip_vertically_on_load(1);
            unsigned char* img_data = stbi_load_from_memory(data, size_b, &w, &h, &bpp, channels);

            if (!img_data)
            {
                handle->SetStatus(ResourceLoadingStatus::CORRUPTED);
                handle->SetFailureReason(ResourceLoadingFailureReason::NO_DATA);
                return;
            }

            auto it = images2d_to_load_async.find(handle->Resource()->Name().data());
            if (it != images2d_to_load_async.end())
            {
                std::mutex mtx;
                std::lock_guard<std::mutex> lock(mtx);
                images2d_to_load_async.unsafe_erase(it);
            }

            Fuego::Graphics::Image2D::Image2DPostCreateion settings{static_cast<uint32_t>(w), static_cast<uint32_t>(h),
                                                                    static_cast<uint16_t>(bpp), channels, img_data};
            handle->Resource()->PostCreate(settings);
            handle->SetStatus(ResourceLoadingStatus::SUCCESS);
            auto image = images2d.emplace(handle->Resource()->Name(), handle->Resource()).first->second;
            FU_CORE_INFO("[AssetsManager] Image was added: name: {0}, ", image->Name());
            ++images2d_count;

            {
                auto it = images2d_to_load_async.find(handle->Resource()->Name().data());
                if (it != images2d_to_load_async.end())
                {
                    std::mutex mtx;
                    std::lock_guard<std::mutex> lock(mtx);
                    images2d_to_load_async.unsafe_erase(it);
                }
            }
        },
        handle, data, size_b, channels);
    return handle;
}
std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> Fuego::AssetsManager::LoadImage2DFromRawData(
    std::string_view name, unsigned char* data, uint32_t channels, uint16_t bpp, uint32_t width, uint32_t height)
{
    if (!data || name.empty())
        return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(ResourceLoadingStatus::CORRUPTED,
                                                                                 ResourceLoadingFailureReason::NO_DATA);

    std::string file_name = std::filesystem::path(name.data()).stem().string();
    std::string ext = std::filesystem::path(name.data()).extension().string();

    auto image = images2d.find(file_name);
    if (image != images2d.end())
        return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(
            image->second, ResourceLoadingStatus::SUCCESS, ResourceLoadingFailureReason::NONE);

    auto img = images2d
                   .emplace(std::move(file_name), std::make_shared<Fuego::Graphics::Image2D>(
                                                      file_name, ext, data, width, height, bpp, channels))
                   .first->second;
    FU_CORE_INFO("[AssetsManager] Image[{0}] was added: name: {1}, width: {2}, height: {3}", ++images2d_count,
                 img->Name(), img->Width(), img->Height());
    return std::make_shared<Fuego::ResourceHandle<Fuego::Graphics::Image2D>>(img, ResourceLoadingStatus::SUCCESS,
                                                                             ResourceLoadingFailureReason::NONE);
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
