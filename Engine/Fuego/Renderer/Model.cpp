#include "Model.h"

#include <assimp/postprocess.h>

#include <assimp/Importer.hpp>
#include <filesystem>

#include "Renderer.h"
#include "assimp/scene.h"
#include "fstream"

Fuego::Graphics::Model::Model(const aiScene* scene)
    : name(std::move(std::filesystem::path(scene->mRootNode->mName.C_Str()).stem().string()))
    , mesh_count(scene->mNumMeshes)
    , vertex_count(0)
    , indices_count(0)
{
    process_model(scene, false);
}

Fuego::Graphics::Model::Model(std::string_view model_name)
    : name(model_name), mesh_count(0), vertex_count(0), indices_count(0)
{
}

Fuego::Graphics::Model::Model(Model&& other) noexcept
    : name(std::move(other.name))
    , mesh_count(other.mesh_count)
    , vertex_count(other.vertex_count)
    , meshes(std::move(other.meshes))
    , indices_count(other.indices_count)
{
    other.mesh_count = 0;
    other.vertex_count = 0;
}

Fuego::Graphics::Model& Fuego::Graphics::Model::operator=(Model&& other) noexcept
{
    if (this != &other)
    {
        name = std::move(other.name);
        mesh_count = other.mesh_count;
        vertex_count = other.vertex_count;
        meshes = std::move(other.meshes);
        indices_count = other.indices_count;

        other.mesh_count = 0;
        other.vertex_count = 0;
        other.indices_count = 0;
    }
    return *this;
}

Fuego::Graphics::Model::Mesh::Mesh(aiMesh* mesh, const Material* material, uint32_t mesh_index,
                                   std::vector<Fuego::Graphics::VertexData>& vertices, std::vector<uint32_t>& indices)
    : mesh_name(mesh->mName.C_Str())
    , vertex_count(mesh->mNumVertices)
    , vertex_start(0)
    , vertex_end(0)
    , index_start(0)
    , index_end(0)
    , indices_count(0)
    , material(material)
{
    vertex_start = vertices.size();
    index_start = indices.size();

    vertices.reserve(vertices.size() + vertex_count);
    for (size_t i = 0; i < vertex_count; i++)
    {
        vertices.emplace_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
        VertexData& vertex = vertices.back();
        if (mesh->HasNormals())
        {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }
        // TODO: mesh may contain up to 8 different texture,
        // use first one for now, later increase textures up to 8?
        if (mesh->HasTextureCoords(0))
        {
            vertex.textcoord.x = mesh->mTextureCoords[0][i].x;
            vertex.textcoord.y = mesh->mTextureCoords[0][i].y;
        }
    }
    indices.reserve(mesh->mNumFaces * 3);
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            uint32_t index = face.mIndices[j] + vertex_start;
            FU_CORE_ASSERT(index >= vertex_start && index < vertex_start + mesh->mNumVertices,
                           "Mesh: index out of range for current mesh vertices");
            indices.push_back(face.mIndices[j] + vertex_start);
        }
        indices_count += face.mNumIndices;
    }
    vertex_end = vertices.size() - 1;
    index_end = indices.size() - 1;
}

void Fuego::Graphics::Model::PostLoad(const aiScene* scene)
{
    process_model(scene);
}

void Fuego::Graphics::Model::process_model(const aiScene* scene, bool async)
{
    mesh_count = scene->mNumMeshes;

    auto renderer = ServiceLocator::instance().GetService<Fuego::Graphics::Renderer>();
    auto assets_manager = ServiceLocator::instance().GetService<Fuego::AssetsManager>();
    meshes.reserve(scene->mNumMeshes);
    materials.reserve(scene->mNumMaterials);

    int texture_index = MAXINT;
    std::map<uint32_t, const Texture*> loaded_textures;
    for (size_t i = 0; i < scene->mNumMaterials; i++)
    {
        aiString path;
        std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> image{};
        if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
        {
            if (path.C_Str()[0] == '*')
            {
                texture_index = atoi(path.C_Str() + 1);
                auto it = loaded_textures.find(texture_index);
                if (it != loaded_textures.end())
                {
                    auto material = Material::CreateMaterial(it->second);
                    materials.emplace_back(std::unique_ptr<Material>(material));
                    continue;
                }

                aiTexture* embeded_texture = scene->mTextures[texture_index];

                uint32_t channels = 0;
                if (embeded_texture->CheckFormat("jpg"))
                    channels = 3;
                else if (embeded_texture->CheckFormat("png"))
                    channels = 4;

                if (embeded_texture->mHeight == 0)
                {
                    std::string name = embeded_texture->mFilename.C_Str();
                    if (embeded_texture->mFilename.length == 0)
                        name = std::string(GetName()) + "_" + "Embeded_txt_" + std::to_string(texture_index);
                    if (async)
                        image = assets_manager->LoadImage2DFromMemoryAsync(
                            name, reinterpret_cast<unsigned char*>(embeded_texture->pcData), embeded_texture->mWidth,
                            channels);
                    else
                        image = assets_manager->LoadImage2DFromMemory(
                            name, reinterpret_cast<unsigned char*>(embeded_texture->pcData), embeded_texture->mWidth,
                            channels);
                }
                else
                    image = assets_manager->LoadImage2DFromRawData(
                        embeded_texture->mFilename.C_Str(), reinterpret_cast<unsigned char*>(embeded_texture->pcData),
                        4, 8, embeded_texture->mWidth, embeded_texture->mHeight);
            }

            else
                image = assets_manager->Load<Image2D>(path.C_Str(), async);

            auto texture = renderer->CreateGraphicsResource<Texture>(image->Resource()->Name());

            // TODO think about passing raw pointer or shared ptr to material
            auto material = Material::CreateMaterial(texture.get());
            materials.emplace_back(std::unique_ptr<Material>(material));
            loaded_textures.emplace(texture_index, texture.get());
        }
        else
        {
            aiColor4D diffuseColor{};
            if (scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
            {
                // TODO we need Color class and creation of solid textures from colors
            }
            auto renderer = ServiceLocator::instance().GetService<Fuego::Graphics::Renderer>();
            // auto texture = renderer->CreateGraphicsResource<Texture>();
            // TODO change fallback texture to solidtexture
            auto material = Material::CreateMaterial(renderer->GetLoadedTexture("fallback").get());
            materials.emplace_back(std::unique_ptr<Material>(material));
        }
    }

    for (size_t i = 0; i < scene->mNumMeshes; i++)
    {
        meshes.emplace_back(std::make_unique<Fuego::Graphics::Model::Mesh>(
            scene->mMeshes[i], materials[scene->mMeshes[i]->mMaterialIndex].get(), i, vertices, indices));
        Fuego::Graphics::Model::Mesh* mesh = meshes.back().get();
        vertex_count += mesh->GetVertexCount();
        indices_count += mesh->GetIndicesCount();
    }
    int a = 5;
}
