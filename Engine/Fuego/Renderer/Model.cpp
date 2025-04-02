#include "Mesh.h"

#include <filesystem>

#include "fstream"


Fuego::Renderer::Mesh::Mesh()
    : vertex_count(0)
    , polygons(0)
    , quads(0)
    , triangles(0)
{
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
        if (line.size() <= 2)
            continue;
        const char first_char = line[0];

        switch (first_char)
        {
        case '#':
            break;
        case 'm':
        {
            if (line.starts_with("mtllib"))
            {
                const char* ptr = line.data();
                ptr += 7;
                char buffer[32] = {0};
                char* buffer_ptr = buffer;
                while (*ptr != '\0')
                {
                    *buffer_ptr += *ptr;
                    ptr++;
                    buffer_ptr++;
                }
                *buffer_ptr = '\0';
                material = std::string(buffer);
            }
            break;
        }

        case 'v':
            ParseVertices(line, vertices_vec, textcoords_vec, normals_vec);
            break;
        case 'f':
            ParseFace(line, faces, textcoords_vec.size() > 0, normals_vec.size() > 0);
            break;
        }
    }
    polygons = faces.size();

    vertices.reserve(vertex_count);

    ProcessFaces(faces, vertices_vec, textcoords_vec, normals_vec, vertices);

    return vertices;
}

Fuego::Renderer::Mesh::Face::Face(glm::ivec3 v_ind, glm::vec2 tx_ind, glm::vec3 n_ind)
    : v_indecies(v_ind)
    , tx_indecies(tx_ind)
    , n_indecies(n_ind)
{
}

void Fuego::Renderer::Mesh::ParseFace(const std::string_view& line, std::vector<Face>& faces, bool hasTextcoord, bool hasNormals)
{
    std::vector<int> v_indices, t_indices, n_indices;
    size_t pos = 2;

    while (pos < line.size())
    {
        int v = -1, t = -1, n = -1;
        size_t next_space = line.find(' ', pos);
        std::string_view vertex = line.substr(pos, next_space - pos);

        if (hasTextcoord && hasNormals)
        {
            sscanf(vertex.data(), "%d/%d/%d", &v, &t, &n);
            v_indices.push_back(v);
            t_indices.push_back(t);
            n_indices.push_back(n);
        }
        else if (hasTextcoord && !hasNormals)
        {
            sscanf(vertex.data(), "%d/%d/", &v, &t);
            v_indices.push_back(v);
            t_indices.push_back(t);
        }
        else if (!hasTextcoord && hasNormals)
        {
            sscanf(vertex.data(), "%d//%d", &v, &n);
            v_indices.push_back(v);
            n_indices.push_back(n);
        }
        else
        {
            sscanf(vertex.data(), "%d", &v);
            v_indices.push_back(v);
        }

        if (next_space == std::string::npos)
            break;
        pos = next_space + 1;
    }

    if (v_indices.size() == 3)
    {
        faces.emplace_back(Face(glm::ivec3(v_indices[0], v_indices[1], v_indices[2]),
                                hasTextcoord ? glm::ivec3(t_indices[0], t_indices[1], t_indices[2]) : glm::ivec3(-1),
                                hasNormals ? glm::ivec3(n_indices[0], n_indices[1], n_indices[2]) : glm::ivec3(-1)));
        vertex_count += 3;
        triangles += 1;
    }
    else if (v_indices.size() == 4)
    {
        faces.emplace_back(Face(glm::ivec3(v_indices[0], v_indices[1], v_indices[2]),
                                hasTextcoord ? glm::ivec3(t_indices[0], t_indices[1], t_indices[2]) : glm::ivec3(-1),
                                hasNormals ? glm::ivec3(n_indices[0], n_indices[1], n_indices[2]) : glm::ivec3(-1)));

        faces.emplace_back(Face(glm::ivec3(v_indices[0], v_indices[2], v_indices[3]),
                                hasTextcoord ? glm::ivec3(t_indices[0], t_indices[2], t_indices[3]) : glm::ivec3(-1),
                                hasNormals ? glm::ivec3(n_indices[0], n_indices[2], n_indices[3]) : glm::ivec3(-1)));

        vertex_count += 6;
        quads += 1;
    }
}


void Fuego::Renderer::Mesh::ParseVertices(const std::string_view& line, std::vector<glm::vec3>& vertecies, std::vector<glm::vec2>& textcoord,
                                          std::vector<glm::vec3>& normals)
{
    float x, y, z;
    const char second_char = line[1];
    switch (second_char)
    {
    case ' ':
        if (sscanf(line.data(), "%*s %f %f %f", &x, &y, &z) == 3)
            ;
        vertecies.emplace_back(glm::vec3(x, y, z));
        break;
    case 't':
        if (sscanf(line.data(), "%*s %f %f", &x, &y) == 2)
            ;
        textcoord.emplace_back(glm::vec2(x, y));
        break;
    case 'n':
        if (sscanf(line.data(), "%*s %f %f %f", &x, &y, &z) == 3)
            ;
        normals.emplace_back(glm::vec3(x, y, z));
        break;
    }
}

void Fuego::Renderer::Mesh::ProcessFaces(const std::vector<Face>& faces, std::vector<glm::vec3>& in_vertices, std::vector<glm::vec2>& in_textcoords,
                                         std::vector<glm::vec3>& in_normals, std::vector<float>& output_vector)
{
    for (const auto& face : faces)
    {
        for (int i = 0; i < 3; ++i)
        {
            Renderer::VertexData vertex;

            vertex.pos = in_vertices[face.v_indecies[i] - 1];

            if (!in_textcoords.empty())
            {
                if (i < 2)
                    vertex.textcoord = in_textcoords[face.tx_indecies[i] - 1];
            }
            else
            {
                vertex.textcoord = glm::vec2(-1.0f);
            }

            if (!in_normals.empty())
            {
                vertex.normal = in_normals[face.n_indecies[i] - 1];
            }
            else
            {
                vertex.normal = glm::vec3(-1.0f);
            }

            output_vector.push_back(vertex.pos.x);
            output_vector.push_back(vertex.pos.y);
            output_vector.push_back(vertex.pos.z);
            output_vector.push_back(vertex.textcoord.x);
            output_vector.push_back(vertex.textcoord.y);
            output_vector.push_back(vertex.normal.x);
            output_vector.push_back(vertex.normal.y);
            output_vector.push_back(vertex.normal.z);
        }
    }
}
