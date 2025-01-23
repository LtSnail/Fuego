#include "FileSystem.h"

#if defined(FUEGO_PLATFORM_WIN)
#include "FileSystemPathsWin.h"
#endif

#if defined(FUEGO_PLATFORM_MACOS)
#include "FileSystemPathsMacOS.h"
#include <limits.h>
#include <mach-o/dyld.h>
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
    const std::vector<std::string_view> _searchPaths = {resource_path.data(), shaders_path.data(), images_path.data(), models_path.data()};
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

}  // namespace Fuego::FS