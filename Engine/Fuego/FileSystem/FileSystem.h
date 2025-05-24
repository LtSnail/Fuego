#pragma once

#include <filesystem>
#include <fstream>

#include "Services/ServiceInterfaces.hpp"

namespace Fuego::Graphics
{
class Model;
}

namespace Fuego::FS
{

class Application;

class FUEGO_API FileSystem : public Service<FileSystem>
{
public:
    friend struct Service<FileSystem>;

    FileSystem(FileSystem&&) noexcept = default;
    FileSystem& operator=(FileSystem&&) noexcept = default;

    std::string OpenFile(const std::string& file, std::fstream::ios_base::openmode mode = std::fstream::ios_base::in);
    const std::string GetFullPathToFile(std::string_view file_name) const;
    std::string GetFullPathToFolder(std::string_view folder_name) const;

    void FUCreateFile(const std::string& file_name, std::string_view folder) const;
    void WriteToFile(std::string_view file_name, const char* buffer);

    friend class Application;
    FileSystem();
    ~FileSystem() = default;
    FUEGO_INTERFACE(FileSystem);

private:
    void OnInit();
    void OnShutdown();
};
}  // namespace Fuego::FS
