#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

using vec3 = glm::vec3;
using mat4 = glm::mat4;

namespace Fuego::Renderer
{
class Renderer;

class Camera
{
public:
    ~Camera();
    FUEGO_NON_COPYABLE_NON_MOVABLE(Camera);


    inline mat4 GetView() const
    {
        return view;
    }
    inline const mat4* GetViewPtr() const
    {
        return &view;
    }
    inline vec3 GetDir() const
    {
        return dir;
    }
    void Activate();
    static inline Camera* GetActiveCamera()
    {
        return active_camera;
    }

    void Update();

private:
    float speed;
    void RotateCamera();
    static Camera* active_camera;
    vec3 position;
    mat4 view;
    vec3 dir;
    vec3 camera_forward;
    vec3 up;

    float yaw;
    float pitch;
    float mouse_sensitivity;

    friend class Renderer;
    Camera();
};
}  // namespace Fuego::Renderer
