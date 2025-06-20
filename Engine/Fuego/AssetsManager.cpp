#include "AssetsManager.h"

#if !defined(CGLTF_IMPLEMENTATION)
#define CGLTF_IMPLEMENTATION
#include "External/cgltf/cgltf.h"
#endif

#include "External/stb_image/stb_image.h"
#include "FileSystem/FileSystem.h"
#include "Services/ServiceLocator.h"

using Model = Fuego::Graphics::Model;
using Texture = Fuego::Graphics::Texture;
using Image2D = Fuego::Graphics::Image2D;

Fuego::AssetsManager::AssetsManager()
    : models_count(0)
    , images2d_count(0)
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

std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Model>> Fuego::AssetsManager::load_model(std::string_view path)
{
    std::shared_ptr<Fuego::ResourceHandle<Model>> handle{nullptr};
    if (path.empty())
        return handle;

    std::string file_name = std::filesystem::path(path).stem().string();
    bool loaded = is_already_loaded(models, file_name, handle);
    if (loaded)
        return handle;

    auto fs = ServiceLocator::instance().GetService<Fuego::FS::FileSystem>();

    handle = std::make_shared<Fuego::ResourceHandle<Model>>(std::make_shared<Model>(file_name));

    auto res = fs->GetFullPathToFile(path);
    if (!res)
    {
        handle->SetCorrupted(WRONG_PATH);
        return handle;
    }

    cgltf_options options = {};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, res->c_str(), &data);
    if (result != cgltf_result_success)
    {
        handle->SetCorrupted(NO_DATA);
        return handle;
    }
    result = cgltf_load_buffers(&options, data, res->c_str());
    std::shared_ptr<Model> model;
    models.emplace(std::move(file_name), handle->Resource());
    ++models_count;
    handle->SetSuccess();

    FU_CORE_INFO("[AssetsManager] Model[{0}] was added: name: {1}, ", models.size(), model->GetName());
    cgltf_free(data);

    return handle;
}
std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Model>> Fuego::AssetsManager::load_model_async(std::string_view path)
{
    std::shared_ptr<Fuego::ResourceHandle<Model>> handle{nullptr};
    if (path.empty())
        return handle;

    std::string file_name = std::filesystem::path(path).stem().string();
    bool loaded = is_already_loaded(models, file_name, handle);
    if (loaded)
        return handle;
    {
        const auto it = models_to_load_async.find(file_name);
        if (it != models_to_load_async.end() && it->second->Status() != CORRUPTED)
            return it->second;
    }

    handle = models_to_load_async.emplace(file_name, std::make_shared<Fuego::ResourceHandle<Model>>(std::make_shared<Model>(file_name))).first->second;

    auto thread_pool = ServiceLocator::instance().GetService<ThreadPool>();

    thread_pool->Submit(
        [this](std::string_view path, std::shared_ptr<Fuego::ResourceHandle<Model>> handle)
        {
            handle->SetStatus(LoadingSts::LOADING);

            auto fs = ServiceLocator::instance().GetService<Fuego::FS::FileSystem>();

            auto res = fs->GetFullPathToFile(path);
            if (!res)
            {
                handle->SetCorrupted(WRONG_PATH);
                return;
            }

            cgltf_options options = {};
            cgltf_data* data = NULL;
            cgltf_result result = cgltf_parse_file(&options, res->c_str(), &data);
            if (result != cgltf_result_success)
            {
                handle->SetCorrupted(NO_DATA);
                return;
            }
            result = cgltf_load_buffers(&options, data, res->c_str());
            handle->Resource()->PostLoad(data);
            auto model = models.emplace(handle->Resource()->GetName(), handle->Resource()).first->second;
            FU_CORE_INFO("[AssetsManager] Model[{0}] was added: name: {1}, ", models.size(), model->GetName());
            ++models_count;
            handle->SetSuccess();

            auto it = models_to_load_async.find(handle->Resource()->GetName().data());
            if (it != models_to_load_async.end())
            {
                std::mutex mtx;
                std::lock_guard<std::mutex> lock(mtx);
                models_to_load_async.unsafe_erase(it);
            }
            cgltf_free(data);
        },
        path, handle);

    return handle;
}

// Image:
std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> Fuego::AssetsManager::load_image2d(std::string_view path)
{
    std::shared_ptr<Fuego::ResourceHandle<Image2D>> handle{nullptr};
    if (path.empty())
        return handle;

    std::string file_name = std::filesystem::path(path).stem().string();
    std::string ext = std::filesystem::path(path).extension().string();

    bool loaded = is_already_loaded(images2d, file_name, handle);
    if (loaded)
        return handle;

    auto fs = ServiceLocator::instance().GetService<Fuego::FS::FileSystem>();

    auto res = fs->GetFullPathToFile(path);
    if (!res)
        return std::make_shared<Fuego::ResourceHandle<Image2D>>(nullptr, CORRUPTED, WRONG_PATH);

    uint16_t channels = ImageChannels(ext);
    stbi_set_flip_vertically_on_load(1);
    int w, h, bpp = 0;
    unsigned char* data = stbi_load(res.value().c_str(), &w, &h, &bpp, channels);
    if (!data)
    {
        FU_CORE_ERROR("Can't load an image: {0} {1}", path, stbi_failure_reason());
        return std::make_shared<Fuego::ResourceHandle<Image2D>>(nullptr, CORRUPTED, NO_DATA);
    }

    auto img = images2d.emplace(file_name, std::make_shared<Image2D>(file_name, ext, data, w, h, bpp, channels)).first->second;
    FU_CORE_INFO("[AssetsManager] Image[{0}] was added: name: {1}, width: {2}, height: {3}", ++images2d_count, img->Name(), img->Width(), img->Height());
    handle = std::make_shared<Fuego::ResourceHandle<Image2D>>(img, SUCCESS);
    return handle;
}

std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> Fuego::AssetsManager::load_image2d_async(std::string_view path)
{
    std::shared_ptr<Fuego::ResourceHandle<Image2D>> handle{nullptr};
    if (path.empty())
        return handle;

    std::string file_name = std::filesystem::path(path).filename().stem().string();
    std::string ext = std::filesystem::path(path).extension().string();

    bool loaded = is_already_loaded(images2d, file_name, handle);
    if (loaded)
        return handle;
    {
        auto it = images2d_to_load_async.find(file_name);
        if (it != images2d_to_load_async.end() && it->second->Status() != CORRUPTED)
            return it->second;
    }

    handle =
        images2d_to_load_async.emplace(file_name, std::make_shared<Fuego::ResourceHandle<Image2D>>(std::make_shared<Image2D>(file_name, ext))).first->second;

    auto thread_pool = ServiceLocator::instance().GetService<ThreadPool>();

    thread_pool->Submit(
        [this](std::shared_ptr<Fuego::ResourceHandle<Image2D>> handle)
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
                handle->SetCorrupted(WRONG_PATH);
                return;
            }

            unsigned char* data = stbi_load(res.value().c_str(), &w, &h, &bpp, channels);

            if (!data)
            {
                handle->SetCorrupted(NO_DATA);
                return;
            }
            Fuego::Graphics::Image2D::Image2DPostCreateion settings{static_cast<uint32_t>(w), static_cast<uint32_t>(h), static_cast<uint16_t>(bpp), channels,
                                                                    data};
            handle->Resource()->PostCreate(settings);

            auto image = images2d.emplace(handle->Resource()->Name(), handle->Resource()).first->second;
            FU_CORE_INFO("[AssetsManager] Image was added: name: {0}, ", image->Name());
            ++images2d_count;
            handle->SetSuccess();

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
std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> Fuego::AssetsManager::LoadImage2DFromMemory(std::string_view name, unsigned char* data,
                                                                                                             uint32_t size_b, uint16_t channels)
{
    std::shared_ptr<Fuego::ResourceHandle<Image2D>> handle{nullptr};
    if (!data)
        return handle;

    std::string file_name = std::filesystem::path(name.data()).stem().string();
    std::string ext = std::filesystem::path(name.data()).extension().string();

    bool loaded = is_already_loaded(images2d, file_name, handle);
    if (loaded)
        return handle;

    int w, h, bpp = 0;
    stbi_set_flip_vertically_on_load(0);
    unsigned char* img_data = stbi_load_from_memory(data, size_b, &w, &h, &bpp, channels);

    if (!img_data)
        return std::make_shared<Fuego::ResourceHandle<Image2D>>(nullptr, CORRUPTED, NO_DATA);

    auto img = images2d.emplace(file_name, std::make_shared<Image2D>(file_name, ext, img_data, w, h, bpp, channels)).first->second;
    FU_CORE_INFO("[AssetsManager] Image[{0}] was added: name: {1}, width: {2}, height: {3}", ++images2d_count, img->Name(), img->Width(), img->Height());
    return std::make_shared<Fuego::ResourceHandle<Image2D>>(img, SUCCESS);
}

std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> Fuego::AssetsManager::LoadImage2DFromMemoryAsync(std::string_view name, unsigned char* data,
                                                                                                                  uint32_t size_b, uint16_t channels)
{
    std::shared_ptr<Fuego::ResourceHandle<Image2D>> handle{nullptr};
    if (!data)
        return handle;

    std::string file_name = std::filesystem::path(name.data()).stem().string();
    std::string ext = std::filesystem::path(name.data()).extension().string();

    bool loaded = is_already_loaded(images2d, file_name, handle);
    if (loaded)
        return handle;

    auto it = images2d_to_load_async.find(file_name);
    if (it != images2d_to_load_async.end() && it->second->Status() != CORRUPTED)
        return it->second;

    handle =
        images2d_to_load_async.emplace(file_name, std::make_shared<Fuego::ResourceHandle<Image2D>>(std::make_shared<Image2D>(file_name, ext))).first->second;

    auto thread_pool = ServiceLocator::instance().GetService<ThreadPool>();
    thread_pool->Submit(
        [this](std::shared_ptr<Fuego::ResourceHandle<Image2D>> handle, unsigned char* data, uint32_t size_b, uint16_t channels)
        {
            if (!data)
            {
                handle->SetCorrupted(NO_DATA);
                return;
            }

            int w, h, bpp = 0;
            stbi_set_flip_vertically_on_load(0);
            unsigned char* img_data = stbi_load_from_memory(data, size_b, &w, &h, &bpp, channels);

            if (!img_data)
            {
                handle->SetCorrupted(NO_DATA);
                return;
            }

            auto it = images2d_to_load_async.find(handle->Resource()->Name().data());
            if (it != images2d_to_load_async.end())
            {
                std::mutex mtx;
                std::lock_guard<std::mutex> lock(mtx);
                images2d_to_load_async.unsafe_erase(it);
            }

            Fuego::Graphics::Image2D::Image2DPostCreateion settings{static_cast<uint32_t>(w), static_cast<uint32_t>(h), static_cast<uint16_t>(bpp), channels,
                                                                    img_data};
            handle->Resource()->PostCreate(settings);
            auto image = images2d.emplace(handle->Resource()->Name(), handle->Resource()).first->second;
            FU_CORE_INFO("[AssetsManager] Image was added: name: {0}, ", image->Name());
            ++images2d_count;
            handle->SetSuccess();

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

std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> Fuego::AssetsManager::LoadImage2DFromRawData(std::string_view name, unsigned char* data,
                                                                                                              uint32_t channels, uint16_t bpp, uint32_t width,
                                                                                                              uint32_t height)
{
    std::shared_ptr<Fuego::ResourceHandle<Image2D>> handle{nullptr};
    if (!data || name.empty())
        return handle;

    std::string file_name = std::filesystem::path(name.data()).stem().string();
    std::string ext = std::filesystem::path(name.data()).extension().string();

    bool loaded = is_already_loaded(images2d, file_name, handle);
    if (loaded)
        return handle;

    auto img = images2d.emplace(file_name, std::make_shared<Image2D>(file_name, ext, data, width, height, bpp, channels)).first->second;
    FU_CORE_INFO("[AssetsManager] Image[{0}] was added: name: {1}, width: {2}, height: {3}", ++images2d_count, img->Name(), img->Width(), img->Height());
    handle = std::make_shared<Fuego::ResourceHandle<Image2D>>(img, SUCCESS);
    return handle;
}

std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> Fuego::AssetsManager::LoadImage2DFromColor(std::string_view name, Fuego::Graphics::Color color,
                                                                                                            uint32_t width, uint32_t height)
{
    if (name.empty())
        return std::shared_ptr<Fuego::ResourceHandle<Image2D>>{nullptr};

    uint32_t channels = Fuego::Graphics::Color::Channels(color);
    size_t size = width * height * channels;

    uint32_t color_data = color.Data();

    unsigned char* data = new unsigned char[size];
    for (size_t i = 0; i < width * height; ++i)
    {
        std::memcpy(data + i * channels, &color_data, channels);
    }
    auto img = images2d.emplace(name, std::make_shared<Image2D>(name, "-", data, width, height, 8, channels)).first->second;

    return std::make_shared<Fuego::ResourceHandle<Image2D>>(img, LoadingSts::SUCCESS);
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
