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

    
    inline glm::mat4 GetView() const
    {
        return view;
    }
    inline const glm::mat4* GetViewPtr() const
    {
        return &view;
    }
    inline glm::vec3 GetDir() const
    {
        return dir;
    }
    void Activate();
    static inline Camera* GetActiveCamera()
    {
        return active_camera;
    }

private:

    float speed;
    void TranslateCamera();
    static Camera* active_camera;
    glm::vec3 position;
    glm::mat4 view;
    glm::vec3 dir;


    friend class Renderer;
    Camera();

    friend class Application;
    void Update();
};
}  // namespace Fuego::Renderer