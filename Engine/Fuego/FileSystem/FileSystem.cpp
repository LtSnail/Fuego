#include "FileSystem/FileSystem.h"

#if defined(FUEGO_PLATFORM_MACOS)
#include <limits.h>
#include <mach-o/dyld.h>
#endif

namespace Fuego::FS
{

std::string FileSystem::OpenFile(const std::string& file, std::fstream::ios_base::openmode mode)
{
    std::string path = pathToResources + pathToShadersWindows + file;
    std::fstream f(path, mode);
    FU_CORE_ASSERT(f.is_open(), "[FS] failed open a file");

    std::stringstream buffer;
    buffer << f.rdbuf();
    f.close();

    return buffer.str();
}

const std::string FileSystem::GetExecutablePath() const
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
    for (const auto& path : _searchPaths)
    {
        std::filesystem::path filePath = pathToResources / std::filesystem::path(path) / fileName;

        if (std::filesystem::exists(filePath))
        {
            return filePath.lexically_normal().string();
        }
    }

    return "";
}

}  // namespace Fuego::FS
