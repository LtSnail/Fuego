#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

namespace Fuego::Renderer
{
class Renderer;

class Camera
{
public:
    ~Camera();
    FUEGO_NON_COPYABLE_NON_MOVABLE(Camera);

    void Update();
    inline glm::mat4 GetView() const
    {
        return view;
    }
    inline glm::vec3 GetDir() const
    {
        return dir;
    }


private:
    glm::mat4 view;
    glm::vec3 dir;

    friend class Renderer;
    Camera();
};
}  // namespace Fuego::Renderer