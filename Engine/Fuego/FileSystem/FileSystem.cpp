#include "FileSystem.h"

#include "External/stb_image/stb_image.h"

#if defined(FUEGO_PLATFORM_WIN)
#include "FileSystemPathsWin.h"
#endif

#if defined(FUEGO_PLATFORM_MACOS)
#include <limits.h>
#include <mach-o/dyld.h>

#include "FileSystemPathsMacOS.h"
#endif

namespace Fuego::FS
{
class FileSystem::FileSystemImpl
{
    friend class FileSystem;
    std::string GetExecutablePath();
    const std::string resource_path = GetExecutablePath() + resource;
    const std::string shaders_path = shaders;
    const std::string images_path = images;
    const std::string models_path = models;
    const std::string scenes_path = scenes;
    const std::vector<std::string_view> _searchPaths = {resource_path.data(), shaders_path.data(), images_path.data(), models_path.data(), scenes_path.data()};
};

FileSystem::FileSystem()
    : d(new FileSystemImpl())
{
}

std::string FileSystem::OpenFile(const std::string& file, std::fstream::ios_base::openmode mode)
{
    std::string path = GetFullPathTo(file);
    std::fstream f(path, mode);
    FU_CORE_ASSERT(f.is_open(), "[FS] failed to open a file");

    std::stringstream buffer;
    buffer << f.rdbuf();
    f.close();

    return buffer.str();
}
unsigned char* FileSystem::Load_Image(const std::string& file, int& x, int& y, int& bits_per_pixel, int image_channels)
{
    std::string path = GetFullPathTo(file);
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(path.c_str(), &x, &y, &bits_per_pixel, image_channels);
    if (!data)
    {
        FU_CORE_ERROR("Can't load an image: {0} {1}", path, stbi_failure_reason());
    }
    return data;
}
std::string FileSystem::FileSystemImpl::GetExecutablePath()
{
#if defined(FUEGO_PLATFORM_WIN)
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
#elif defined(FUEGO_PLATFORM_MACOS)
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0)
    {
        throw std::runtime_error("Buffer too small for executable path");
    }
#endif
    return std::filesystem::path(path).parent_path().string();
}
const std::string FileSystem::GetFullPathTo(std::string_view fileName) const
{
    std::filesystem::path file(fileName);
    if (std::filesystem::exists(file))
    {
        std::string extension = file.extension().string();
        if (extension == "png" || extension == "jpg")
        {
            std::filesystem::path filePath = d->resource_path / std::filesystem::path(images) / fileName;
            if (std::filesystem::exists(filePath))
            {
                return filePath.lexically_normal().string();
            }
        }
    }

    for (const auto& path : d->_searchPaths)
    {
        std::filesystem::path filePath = d->resource_path / std::filesystem::path(path) / fileName;

        if (std::filesystem::exists(filePath))
        {
            return filePath.lexically_normal().string();
        }
    }

    return "";
}
void FileSystem::FUCreateFile(const std::string& file_name, const std::string& folder) const
{
    const std::string folder = GetFullPathTo(folder);
    if (!std::filesystem::exists(folder))
    {
        std::filesystem::create_directories(folder);
    }
    std::ofstream file(file_name);
    if (file)
    {
        file.close();
    }
    else
    {
        FU_CORE_ERROR("Failed to create file");
    }
}

}  // namespace Fuego::FS
