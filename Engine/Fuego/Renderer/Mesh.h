#pragma once

#include "Renderer.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"

namespace Fuego::Renderer
{
class Mesh
{
public:
    Mesh();
    ~Mesh() = default;

    std::vector<float> load(const char* name);
    inline unsigned short int GetVertexCount() const
    {
        return vertex_count;
    }

private:
    class Face
    {
    public:
        glm::ivec3 v_indecies;
        glm::ivec2 tx_indecies;
        glm::ivec3 n_indecies;

        Face(glm::ivec3 v_ind, glm::vec2 tx_ind, glm::vec3 n_ind);
    };

    void ParseFace(const std::string_view& line, std::vector<Face>& faces, bool hasTextcoord, bool hasNormals);
    void ParseVertices(const std::string_view& line, std::vector<glm::vec3>& vertecies, std::vector<glm::vec2>& textcoord, std::vector<glm::vec3>& normals);
    void ProcessFaces(const std::vector<Face>& faces, std::vector<glm::vec3>& in_vertices, std::vector<glm::vec2>& in_textcoords,
                      std::vector<glm::vec3>& in_normals, std::vector<float>& output_vector);

    std::string model_name;
    std::string material;
    unsigned short int vertex_count;
    unsigned short int polygons;
    unsigned short int quads;
    unsigned short int triangles;
};
}  // namespace Fuego::Renderer
