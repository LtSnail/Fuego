#include "Mesh.h"

#include <filesystem>

#include "fstream"

bool Fuego::Renderer::Mesh::load(const char* name)
{
    std::filesystem::path ph = name;
    model_name = ph.filename().string().c_str();

    std::vector<std::string> lines;
    std::string line;

    std::fstream f;
    f.open(name);
    if (!f.is_open())
    {
        FU_CORE_ERROR("Cannot open a file");
        return false;
    }
    while (!f.eof())
    {
        std::getline(f, line);
        lines.emplace_back(line);
    }
    f.close();

    float a, b, c;

    for (std::string_view line : lines)
    {
        if (line.size() == 0)
            continue;

        if (line[0] == 'v')
        {
            if (line[1] == ' ')
            {
                sscanf(line.data(), "v  %f %f %f", &a, &b, &c);
                _vertices.emplace_back(new float[3]{a, b, c});
            }
            else if (line[1] == 't')
            {
                sscanf(line.data(), "vt %f %f", &a, &b);
                _textcoords.emplace_back(new float[2]{a, b});
            }
            else if (line[1] == 'n')
            {
                sscanf(line.data(), "vn %f %f %f", &a, &b, &c);
                _normals.emplace_back(new float[3]{a, b, c});
            }
        }
        else if (line[0] == 'f')
        {
            int v0, v1, v2, t0, t1, t2, n;
            sscanf(line.data(), "f %d/%d%d%d%d%d%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n);
            int* v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
            int* t = new int[3]{t0 - 1, t1 - 1, t2 - 1};
            _faces.emplace_back(Face(3, v, t, n - 1));
        }
    }
    return true;
}
