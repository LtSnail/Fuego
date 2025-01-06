#pragma once

#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "Renderer.h"

namespace Fuego::Renderer
{
class Mesh
{
public:
    Mesh();
    ~Mesh();

    void draw();
    std::vector<float> load(const char* name);
    void Release();
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

        Face(glm::ivec3 v_ind, glm::vec2 tx_ind, glm::vec3 n_ind)
            : v_indecies(v_ind)
            , tx_indecies(tx_ind)
            , n_indecies(n_ind)
        {
        }
    };

   
    float GetFloatFromCString(const char*& str);
    void ParseFace(const char*& str, OUT int& vertex, OUT int& texture, OUT int& normal);

    //std::vector<Renderer::VertexData> vertices;

    std::string model_name;
    unsigned int list;
    unsigned short int vertex_count;
    unsigned short int polygons;
};
}  // namespace Fuego::Renderer
