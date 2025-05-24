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
    class FUEGO_API Mesh
    {
    public:
        Mesh(aiMesh* mesh, const Material* material, uint32_t mesh_index, std::vector<Fuego::Graphics::VertexData>& vertices, std::vector<uint32_t>& indices);
        ~Mesh() = default;

        inline uint32_t GetVertexCount() const
        {
            return vertex_count;
        }
        inline uint32_t GetIndicesCount() const
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

        inline void SetMaterial(const Material* material)
        {
            this->material = material;
        }
        inline const Material* GetMaterial() const
        {
            return material;
        }
        inline std::string_view Name() const
        {
            return mesh_name;
        }

    private:
        std::string mesh_name;

        const Material* material;

        uint32_t vertex_start;
        uint32_t vertex_end;

        uint32_t index_start;
        uint32_t index_end;

        uint32_t vertex_count;
        uint32_t indices_count;
    };

    Model(const aiScene* scene);
    ~Model() = default;

    Model(Model&& other) noexcept;
    Model& operator=(Model&& other) noexcept;

    inline std::string_view GetName() const
    {
        return name;
    }
    inline uint32_t GetMeshCount() const
    {
        return mesh_count;
    }
    inline uint32_t GetVertexCount() const
    {
        return vertex_count;
    }
    inline uint32_t GetIndicesCount() const
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
    const std::vector<std::unique_ptr<Fuego::Graphics::Model::Mesh>>* GetMeshesPtr() const
    {
        return &meshes;
    }

private:
    std::string name;
    uint32_t mesh_count;
    uint32_t vertex_count;
    uint32_t indices_count;
    std::vector<Fuego::Graphics::VertexData> vertices;
    std::vector<uint32_t> indices;
    std::vector<std::unique_ptr<Material>> materials;
    std::vector<std::unique_ptr<Model::Mesh>> meshes;
};

}  // namespace Fuego::Graphics
