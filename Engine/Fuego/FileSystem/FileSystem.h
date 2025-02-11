#pragma once
#include <filesystem>
#include <fstream>

namespace Fuego::FS
{

class Application;

class FUEGO_API FileSystem
{
public:
    FUEGO_NON_COPYABLE_NON_MOVABLE(FileSystem)

    std::string OpenFile(const std::string& file, std::fstream::ios_base::openmode mode = std::fstream::ios_base::in);
    unsigned char* Load_Image(const std::string& file, int& x, int& y, int& bits_per_pixel, int image_channels = 0);
    const std::string GetFullPathTo(std::string_view fileName) const;

    void FUCreateFile(const std::string& file_name, const std::string& folder) const;


    friend class Application;
    FileSystem();
    ~FileSystem() = default;
    FUEGO_INTERFACE(FileSystem);
};
}  // namespace Fuego::FS
