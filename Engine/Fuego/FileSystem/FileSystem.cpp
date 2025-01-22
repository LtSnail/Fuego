#include "FileSystem.h"

#if defined(FUEGO_PLATFORM_MACOS)
#include <limits.h>
#include <mach-o/dyld.h>
#endif

namespace Fuego::FS
{
class FileSystem::FileSystemImpl
{
    friend class FileSystem;
    std::string GetExecutablePath();
    FileSystemImpl();
    std::string pathToResources;
    std::string pathToShadersWindows;
    std::string pathToImagesWindows;
    std::string pathToModelsWindows;
    std::vector<std::string_view> _searchPaths;
};

FileSystem::FileSystemImpl::FileSystemImpl()
{
    pathToResources = GetExecutablePath() + "\\..\\..\\..\\..\\Sandbox\\Resources\\";
    pathToShadersWindows = "Windows\\Shaders\\";
    pathToImagesWindows = "Windows\\Images\\";
    pathToModelsWindows = "Windows\\Models\\";
    _searchPaths = {pathToShadersWindows.data(), pathToImagesWindows.data(), pathToModelsWindows.data()};
}

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
        std::filesystem::path filePath = d->pathToResources / std::filesystem::path(path) / fileName;

        if (std::filesystem::exists(filePath))
        {
            return filePath.lexically_normal().string();
        }
    }

    return "";
}

}  // namespace Fuego::FS