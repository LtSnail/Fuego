#include "Mesh.h"

#include <filesystem>

#include "fstream"


Fuego::Renderer::Mesh::Mesh()
    : vertex_count(0)
{
}

Fuego::Renderer::Mesh::~Mesh()
{
    Release();
}



std::vector<float> Fuego::Renderer::Mesh::load(const char* name)
{
    int condition = 0;
    std::vector<float> vertices;
    std::vector<glm::vec3> vertices_vec;
    std::vector<glm::vec2> textcoords_vec;
    std::vector<glm::vec3> normals_vec;
    std::vector<Face> faces;

    std::filesystem::path ph = name;
    model_name = ph.filename().string().c_str();

    std::vector<std::string> lines;
    std::string line;

    std::fstream f;
    f.open(name);
    if (!f.is_open())
    {
        FU_CORE_ERROR("Cannot open a file");
    }
    while (std::getline(f, line))
    {
        lines.emplace_back(line);
    }
    f.close();

    for (std::string_view line : lines)
    {
        if (line.size() <= 2) continue;
        const char first_char = line[0];
        const char second_char = line[1];
        switch (first_char)
        {
        case '#':
            break;
        case 'v':
            float x, y, z;
            switch (second_char)
            {
            case ' ':
                sscanf(line.data(), "%*s %f %f %f", &x, &y, &z);
                vertices_vec.emplace_back(glm::vec3(x, y, z));
                break;
            case 't':
                sscanf(line.data(), "%*s %f %f", &x, &y);
                textcoords_vec.emplace_back(glm::vec2(x, y));
                break;
            case 'n':
                sscanf(line.data(), "%*s %f %f %f", &x, &y, &z);
                normals_vec.emplace_back(glm::vec3(x, y, z));
                break;
            break;
            }
            break;
        case 'f':
            std::vector<int> v_indices, t_indices, n_indices;
            size_t pos = 2;

            while (pos < line.size())
            {
                int v = -1, t = -1, n = -1;
                size_t next_space = line.find(' ', pos);
                std::string_view vertex = line.substr(pos, next_space - pos);

                if (sscanf(vertex.data(), "%d/%d/%d", &v, &t, &n) == 3)
                {
                    v_indices.push_back(v - 1);
                    t_indices.push_back(t - 1);
                    n_indices.push_back(n - 1);
                }
                else if (sscanf(vertex.data(), "%d/%d/", &v, &t) == 2)
                {
                    v_indices.push_back(v - 1);
                    t_indices.push_back(t - 1);
                }
                else if (sscanf(vertex.data(), "%d//%d", &v, &n) == 2)
                {
                    v_indices.push_back(v - 1);
                    n_indices.push_back(n - 1);
                }
                else if (sscanf(vertex.data(), "%d", &v) == 1)
                {
                    v_indices.push_back(v - 1);
                }

                if (next_space == std::string::npos)
                    break;
                pos = next_space + 1;
            }
            
            if (!t_indices.empty())
                condition |= 1;
            if (!n_indices.empty())
                condition |= 2;

            if (v_indices.size() == 3)
            {
                faces.emplace_back(Face(glm::ivec3(v_indices[0], v_indices[1], v_indices[2]),
                                        condition & 1 ? glm::ivec3(t_indices[0], t_indices[1], t_indices[2]) : glm::ivec3(-1),
                                        condition & 2 ? glm::ivec3(n_indices[0], n_indices[1], n_indices[2]) : glm::ivec3(-1)));
                vertex_count +=3;

            }
            else if (v_indices.size() == 4)
            {
                // Triangulate quad into two faces
                faces.emplace_back(Face(glm::ivec3(v_indices[0], v_indices[1], v_indices[2]),
                                        condition & 1 ? glm::ivec3(t_indices[0], t_indices[1], t_indices[2]) : glm::ivec3(-1),
                                        condition & 2 ? glm::ivec3(n_indices[0], n_indices[1], n_indices[2]) : glm::ivec3(-1)));

                faces.emplace_back(Face(glm::ivec3(v_indices[0], v_indices[2], v_indices[3]),
                                        condition & 1 ? glm::ivec3(t_indices[0], t_indices[2], t_indices[3]) : glm::ivec3(-1),
                                        condition & 2 ? glm::ivec3(n_indices[0], n_indices[2], n_indices[3]) : glm::ivec3(-1)));
                vertex_count += 6;
            }
        }
    }
    polygons = faces.size();

    vertices.reserve(vertex_count);
    for (const auto& face : faces)
    {
        for (int i = 0; i < 3; ++i)
        {
            Renderer::VertexData vertex;
            vertex.pos = vertices_vec[face.v_indecies[i]];
            vertices.push_back(vertex.pos.x);
            vertices.push_back(vertex.pos.y);
            vertices.push_back(vertex.pos.z);
            if (condition & 1)  // Textcoords exist
            {
                vertex.textcoord = textcoords_vec[face.tx_indecies[i % 2]];
                vertices.push_back(vertex.textcoord.x);
                vertices.push_back(vertex.textcoord.y);
            }

            if (condition & 2)  // Normals exist
            {
                vertex.normal = normals_vec[face.n_indecies[i]];
                vertices.push_back(vertex.normal.x);
                vertices.push_back(vertex.normal.y);
                vertices.push_back(vertex.normal.z);
            }
        }
    }
    return vertices;
}

void Fuego::Renderer::Mesh::Release()
{
    /*for (float* v : vertices_vec)
    {
        delete[] v;
    }
    vertices_vec.clear();

    for (float* t : textcoords_vec)
    {
        delete[] t;
    }
    textcoords_vec.clear();

    for (float* n : normals_vec)
    {
        delete[] n;
    }
    normals_vec.clear();

    for (Face& face : faces)
    {
        delete[] face._vertices;
        delete[] face._textcoords;
    }
    faces.clear();*/
}


// Parse C-style string till first ' ', '/' occurance
// Input: pointer to first float char or sign '-'
float Fuego::Renderer::Mesh::GetFloatFromCString(const char*& str)
{
    char buffer[16] = {'0'};
    char* bptr = buffer;

    do
    {
        *bptr++ = *str++;
    } while (*str != ' ' && *str != '\0' && *str != '/');

    return std::stof(buffer);
}

// Input: pointer to first value of Face
void Fuego::Renderer::Mesh::ParseFace(const char*& str, OUT int& vertex, OUT int& texture, OUT int& normal)
{
    vertex = (int)GetFloatFromCString(str) - 1;
    str++;
    if (*str == '/')
        texture = -1;
    else
        texture = (int)GetFloatFromCString(str) - 1;
    str++;
    if (*str == '/')
        normal = -1;
    else
        normal = (int)GetFloatFromCString(str) - 1;
}

void Fuego::Renderer::Mesh::draw()
{

}