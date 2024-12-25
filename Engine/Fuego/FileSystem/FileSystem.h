#pragma once
#include <fstream>
#include <filesystem>
#include "string.h"

namespace Fuego::FS
{
const std::string pathToResources = std::filesystem::current_path().string() + "\\..\\..\\..\\Sandbox\\Resources\\";
const std::string pathToShadersWindows = "Windows\\Shaders\\";

class Application;

class FileSystem
{
public:
    std::string OpenFile(const std::string& file, std::fstream::ios_base::openmode mode = std::fstream::ios_base::in);

friend class Application;
    FileSystem() = default;
    ~FileSystem() = default;

    FUEGO_NON_COPYABLE_NON_MOVABLE(FileSystem)
};
}