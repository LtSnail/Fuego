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
    const std::vector<std::string_view> _searchPaths = {shaders_path.data(), images_path.data(), models_path.data(), scenes_path.data()};
};

FileSystem::FileSystem()
    : d(new FileSystemImpl())
{
}

std::string FileSystem::OpenFile(const std::string& file, std::fstream::ios_base::openmode mode)
{
    std::string path = GetFullPathToFile(file);
    std::fstream f(path, mode);
    FU_CORE_ASSERT(f.is_open(), "[FS] failed to open a file");

    std::stringstream buffer;
    buffer << f.rdbuf();
    f.close();

    return buffer.str();
}
unsigned char* FileSystem::Load_Image(const std::string& file, int& x, int& y, int& bits_per_pixel, int image_channels)
{
    std::string path = GetFullPathToFile(file);
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
const std::string FileSystem::GetFullPathToFile(std::string_view fileName) const
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
std::string FileSystem::GetFullPathToFolder(std::string_view folder_name) const
{
    for (const auto& path : d->_searchPaths)
    {
        std::filesystem::path folder_path = ((d->resource_path / std::filesystem::path(path)).lexically_normal());
        if (std::filesystem::exists(folder_path) && std::filesystem::is_directory(folder_path))
        {
            std::string folder = folder_path.parent_path().filename().string();
            if (folder.compare(folder_name.data()) == 0)
            {
                return folder_path.string();
            }
        }
    }
    return "";
}
void FileSystem::FUCreateFile(const std::string& file_name, std::string_view folder) const
{
    const std::filesystem::path path_to_folder = GetFullPathToFolder(folder);
    if (!std::filesystem::exists(path_to_folder))
    {
        FU_CORE_ERROR("Failed to create file");
        return;
    }
    std::ofstream file(path_to_folder / file_name);
    if (file)
    {
        file.close();
    }
}
void FileSystem::WriteToFile(std::string_view file_name, const char* buffer)
{
    if (!buffer || file_name.empty())
        return;

    const std::string path_to_folder = GetFullPathToFile(file_name);
    if (!std::filesystem::exists(path_to_folder))
    {
        FU_CORE_ERROR("[FileSystem] File doesn't exist");
        return;
    }
    std::ofstream file(path_to_folder, std::ios::out | std::ios::trunc);
    if (file)
    {
        file << buffer << std::endl;
        file.close();
    }
}

}  // namespace Fuego::FS
