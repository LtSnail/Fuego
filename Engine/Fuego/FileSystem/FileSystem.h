#pragma once
#include "Renderer.h"
#include <filesystem>
#include <fstream>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace Fuego
{
    class Application;

    namespace Renderer
    {
        class Model;
    }
}

namespace Fuego::FS
{
class FUEGO_API FileSystem
{
public:
    FUEGO_NON_COPYABLE_NON_MOVABLE(FileSystem)

    std::string OpenFile(const std::string& file, std::fstream::ios_base::openmode mode = std::fstream::ios_base::in);

    unsigned char* Load_Image(const std::string& file, int& x, int& y, int& bits_per_pixel, int image_channels = 0);

    Fuego::Renderer::Model* LoadModel(std::string_view path);

    const std::string GetFullPathToFile(std::string_view file_name) const;
    std::string GetFullPathToFolder(std::string_view folder_name) const;

    void FUCreateFile(const std::string& file_name, std::string_view folder) const;
    void WriteToFile(std::string_view file_name, const char* buffer);


    friend class Application;
    FileSystem();
    ~FileSystem() = default;
    FUEGO_INTERFACE(FileSystem);

private:
    void Processnode(aiNode* node, const aiScene* scene);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
};
}  // namespace Fuego::FS
