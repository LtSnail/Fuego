#pragma once

namespace Fuego::Renderer
{
class Mesh
{
public:
    bool load(const char* name);

private:
    class Face
    {
    public:
        int _edge;
        int* _vertices;
        int* _textcoords;
        int _normal;

        Face(int edge, int* vertices, int* textcoords, int normal = -1)
            : _edge(edge)
            , _vertices(vertices)
            , _textcoords(textcoords)
            , _normal(normal)
        {
        }
    };

    std::vector<float*> _vertices;
    std::vector<float*> _textcoords;
    std::vector<float*> _normals;
    std::vector<Face> _faces;

    std::string model_name;
};
}  // namespace Fuego::Renderer
