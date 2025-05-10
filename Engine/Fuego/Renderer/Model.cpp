#include "Model.h"

#include <assimp/postprocess.h>

#include <assimp/Importer.hpp>
#include <filesystem>

#include "Renderer.h"
#include "assimp/scene.h"
#include "fstream"

Fuego::Graphics::Model::Model(const aiScene* scene)
    : name(scene->mRootNode->mName.C_Str())
    , mesh_count(scene->mNumMeshes)
    , vertex_count(0)
    , indices_count(0)
{
    meshes.reserve(scene->mNumMeshes);
    for (size_t i = 0; i < scene->mNumMeshes; i++)
    {
        meshes.emplace_back(
            std::make_unique<Fuego::Graphics::Model::Mesh>(scene->mMeshes[i], scene->mMaterials[scene->mMeshes[i]->mMaterialIndex], i, vertices, indices));
        Fuego::Graphics::Model::Mesh* mesh = meshes.back().get();
        vertex_count += mesh->GetVertexCount();
        indices_count += mesh->GetIndicesCount();
        if (!Application::instance().IsTextureLoaded(mesh->GetTextureName()))
        {
            Application::instance().AddTexture(mesh->GetTextureName());
        }
        mesh->SetMaterial(Material::CreateMaterial(Application::instance().GetLoadedTexture(mesh->GetTextureName())));
    }
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

Fuego::Graphics::Model::Mesh::Mesh(aiMesh* mesh, aiMaterial* material, uint16_t mesh_index, std::vector<Fuego::Graphics::VertexData>& vertices,
                                   std::vector<uint32_t>& indices)
    : mesh_name(mesh->mName.C_Str())
    , vertex_count(mesh->mNumVertices)
    , vertex_start(0)
    , vertex_end(0)
    , index_start(0)
    , index_end(0)
    , indices_count(0)
    , texture("")
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
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j] + vertex_start);
        }
        indices_count += face.mNumIndices;
    }
    vertex_end = vertices.size() - 1;
    index_end = indices.size() - 1;
    aiString path;
    material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
    texture = path.C_Str();
}
