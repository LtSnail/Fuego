#pragma once
#include <fstream>
#include <filesystem>
#include "string.h"

namespace Fuego::FS
{


class Application;

class FileSystem
{
public:
    FUEGO_NON_COPYABLE_NON_MOVABLE(FileSystem)

    std::string OpenFile(const std::string& file, std::fstream::ios_base::openmode mode = std::fstream::ios_base::in);
    const std::string GetExecutablePath() const;


    friend class Application;
    FileSystem() = default;
    ~FileSystem() = default;

private:
    const std::string pathToResources = GetExecutablePath() + "\\..\\..\\..\\..\\Sandbox\\Resources\\";
    const std::string pathToShadersWindows = "Windows\\Shaders\\";
};
}