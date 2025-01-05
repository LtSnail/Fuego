#pragma once
#include <filesystem>
#include <fstream>

namespace Fuego::FS
{


class Application;

class FileSystem
{
public:
    FUEGO_NON_COPYABLE_NON_MOVABLE(FileSystem)

    std::string OpenFile(const std::string& file, std::fstream::ios_base::openmode mode = std::fstream::ios_base::in);
    const std::string GetExecutablePath() const;
    const std::string GetFullPathTo(std::string_view fileName) const;


    friend class Application;
    FileSystem() = default;
    ~FileSystem() = default;

private:
    const std::string pathToResources = GetExecutablePath() + "\\..\\..\\..\\..\\Sandbox\\Resources\\";
    const std::string pathToShadersWindows = "Windows\\Shaders\\";
    const std::string pathToImagesWindows = "Windows\\Images\\";
    const std::vector<std::string_view> _searchPaths = {pathToShadersWindows.data(), pathToImagesWindows.data()};
};
}  // namespace Fuego::FS
