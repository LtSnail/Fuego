#pragma once

#include "glm/ext.hpp"
#include "glm/glm.hpp"

struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace Fuego::Graphics
{
struct VertexData;
class Material;

class FUEGO_API Model
{
public:
    Model(const aiScene* scene);
    ~Model() = default;

    Model(Model&& other) noexcept;
    Model& operator=(Model&& other) noexcept;

    inline std::string_view GetName() const
    {
        return name;
    }
    inline uint16_t GetMeshCount() const
    {
        return mesh_count;
    }
    inline uint16_t GetVertexCount() const
    {
        return vertex_count;
    }
    inline uint16_t GetIndicesCount() const
    {
        return indices_count;
    }

    inline const VertexData* GetVerticesData() const
    {
        return vertices.data();
    }
    inline const uint32_t* GetIndicesData() const
    {
        return indices.data();
    }


private:
    std::string name;
    uint16_t mesh_count;
    uint32_t vertex_count;
    uint32_t indices_count;
    std::vector<Fuego::Graphics::VertexData> vertices;
    std::vector<uint32_t> indices;

public:
    class FUEGO_API Mesh
    {
    public:
        Mesh(aiMesh* mesh, aiMaterial* material, uint16_t mesh_index, std::vector<Fuego::Graphics::VertexData>& vertices, std::vector<uint32_t>& indices);
        ~Mesh() = default;

        inline std::string_view GetTextureName() const
        {
            return texture;
        }

        inline uint16_t GetVertexCount() const
        {
            return vertex_count;
        }
        inline uint16_t GetIndicesCount() const
        {
            return indices_count;
        }

        inline uint32_t GetVertexStart() const
        {
            return vertex_start;
        }
        inline uint32_t GetVertexEnd() const
        {
            return vertex_end;
        }

        inline uint32_t GetIndexStart() const
        {
            return index_start;
        }
        inline uint32_t GetIndexEnd() const
        {
            return index_end;
        }

        inline uint32_t GetVertexSize() const
        {
            return vertex_count * sizeof(float);
        }
        inline uint32_t GetIndexSize() const
        {
            return indices_count * sizeof(uint32_t);
        }

        inline void SetMaterial(Material* material)
        {
            this->material.reset(material);
        }

        inline Material* GetMaterial() const
        {
            return material.get();
        }

    private:
        std::string mesh_name;
        std::unique_ptr<Material> material;

        uint32_t vertex_start;
        uint32_t vertex_end;

        uint32_t index_start;
        uint32_t index_end;

        uint16_t vertex_count;
        uint16_t indices_count;

        std::string texture;
    };

private:
    std::vector<std::unique_ptr<Model::Mesh>> meshes;

public:
    const std::vector<std::unique_ptr<Fuego::Graphics::Model::Mesh>>* GetMeshesPtr() const
    {
        return &meshes;
    }
};

}  // namespace Fuego::Graphics
