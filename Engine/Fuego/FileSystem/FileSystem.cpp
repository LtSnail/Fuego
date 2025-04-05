#include "FileSystem.h"

#include "External/stb_image/stb_image.h"
#include "Model.h"
#if defined(FUEGO_PLATFORM_WIN)
#include "FileSystemPathsWin.h"
#endif

#if defined(FUEGO_PLATFORM_MACOS)
#include <limits.h>
#include <mach-o/dyld.h>

#include "FileSystemPathsMacOS.h"
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
    const std::string scenes_path = scenes;
    const std::vector<std::string_view> _searchPaths = {shaders_path.data(), images_path.data(), models_path.data(), scenes_path.data()};
};

FileSystem::FileSystem()
    : d(new FileSystemImpl())
{
}

void FileSystem::Processnode(aiNode* node, const aiScene* scene)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        Processnode(node->mChildren[i], scene);
    }
}

void FileSystem::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        if (mesh->HasNormals())
        {
            vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        }
        if (mesh->HasTextureCoords(0))
        {
            vertex.texCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
}

std::string FileSystem::OpenFile(const std::string& file, std::fstream::ios_base::openmode mode)
{
    std::string path = GetFullPathToFile(file);
    std::fstream f(path, mode);
    FU_CORE_ASSERT(f.is_open(), "[FS] failed to open a file");

    std::stringstream buffer;
    buffer << f.rdbuf();
    f.close();

    return buffer.str();
}
unsigned char* FileSystem::Load_Image(const std::string& file, int& x, int& y, int& bits_per_pixel, int image_channels)
{
    std::string path = GetFullPathToFile(file);
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(path.c_str(), &x, &y, &bits_per_pixel, image_channels);
    if (!data)
    {
        FU_CORE_ERROR("Can't load an image: {0} {1}", path, stbi_failure_reason());
    }
    return data;
}

Fuego::Renderer::Model* FileSystem::LoadModel(std::string_view path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.data(), aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    Processnode(scene->mRootNode, scene);
    return new Fuego::Renderer::Model();
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
const std::string FileSystem::GetFullPathToFile(std::string_view fileName) const
{
    std::filesystem::path file(fileName);
    if (std::filesystem::exists(file))
    {
        std::string extension = file.extension().string();
        if (extension == "png" || extension == "jpg")
        {
            std::filesystem::path filePath = d->resource_path / std::filesystem::path(images) / fileName;
            if (std::filesystem::exists(filePath))
            {
                return filePath.lexically_normal().string();
            }
        }
    }

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
std::string FileSystem::GetFullPathToFolder(std::string_view folder_name) const
{
    for (const auto& path : d->_searchPaths)
    {
        std::filesystem::path folder_path = ((d->resource_path / std::filesystem::path(path)).lexically_normal());
        if (std::filesystem::exists(folder_path) && std::filesystem::is_directory(folder_path))
        {
            std::string folder = folder_path.parent_path().filename().string();
            if (folder.compare(folder_name.data()) == 0)
            {
                return folder_path.string();
            }
        }
    }
    return "";
}
void FileSystem::FUCreateFile(const std::string& file_name, std::string_view folder) const
{
    const std::filesystem::path path_to_folder = GetFullPathToFolder(folder);
    if (!std::filesystem::exists(path_to_folder))
    {
        FU_CORE_ERROR("Failed to create file");
        return;
    }
    std::ofstream file(path_to_folder / file_name);
    if (file)
    {
        file.close();
    }
}
void FileSystem::WriteToFile(std::string_view file_name, const char* buffer)
{
    if (!buffer || file_name.empty())
        return;

    const std::string path_to_folder = GetFullPathToFile(file_name);
    if (!std::filesystem::exists(path_to_folder))
    {
        FU_CORE_ERROR("[FileSystem] File doesn't exist");
        return;
    }
    std::ofstream file(path_to_folder, std::ios::out | std::ios::trunc);
    if (file)
    {
        file << buffer << std::endl;
        file.close();
    }
}

}  // namespace Fuego::FS
