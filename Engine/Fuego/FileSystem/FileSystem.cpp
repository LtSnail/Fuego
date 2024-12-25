#include "FileSystem/FileSystem.h"

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

}  // namespace Fuego::FS