#include "Camera.h"

#include "Input.h"

#define MOUSE_WHEEL_SCROLL_SPEED 5.f

namespace Fuego::Graphics
{

Camera* Camera::active_camera = nullptr;

Camera::Camera()
    : speed(50.1)
    , position(0.0f)
    , up(glm::vec3(0.0f, 1.0f, 0.0f))
    , yaw(0.0f)
    , pitch(0.0f)
    , mouse_sensitivity(10.f)
    , camera_forward(0.0f, 0.0f, -1.0f)
    , view(glm::mat4(1.0f))
    , dir(glm::vec3(0.0f, 0.0f, 0.0f))
    , projection(glm::mat4(1.0f))
    , FOV(60)
    , near_clip(0.1f)
    , far_clip(3000.0f)

{
}

Camera::~Camera()
{
}

void Camera::Activate()
{
    active_camera = this;
}

const mat4* Camera::GetViewPtr() const
{
    return &view;
}
vec3 Camera::GetDir() const
{
    return dir;
}
Camera* Camera::GetActiveCamera()
{
    return active_camera;
}

void Camera::OnUpdate(float dlTime)
{
    if (Input::IsKeyPressed(Key::W))
    {
        position += speed * camera_forward * dlTime;
    }
    if (Input::IsKeyPressed(Key::S))
    {
        position -= speed * camera_forward * dlTime;
    }
    if (Input::IsKeyPressed(Key::A))
    {
        position -= glm::normalize(glm::cross(camera_forward, up)) * speed * dlTime;
    }
    if (Input::IsKeyPressed(Key::D))
    {
        position += glm::normalize(glm::cross(camera_forward, up)) * speed * dlTime;
    }

    std::pair<float, float> mouse_wheel_scroll;
    if (Input::IsMouseWheelScrolled(mouse_wheel_scroll))
    {
        if (mouse_wheel_scroll.first != 0.f)
        {
            if (mouse_wheel_scroll.first > 0.f)
                speed += MOUSE_WHEEL_SCROLL_SPEED;
            else
                speed -= MOUSE_WHEEL_SCROLL_SPEED;
            speed = std::clamp(speed, 1.f, 300.f);
        }
    }

    RotateCamera(dlTime);
    projection = glm::perspective(glm::radians((float)FOV), 1280.0F / 720.0F, near_clip, far_clip);
    view = glm::lookAt(position, position + camera_forward, up);
}

void Camera::OnPostUpdate(float dlTime)
{
    // TODO
}

void Camera::OnFixedUpdate()
{
    // TODO
}

void Camera::RotateCamera(float dtTime)
{
    glm::vec2 mouse_dir = Input::GetMouseDir();
    yaw += mouse_dir.x * mouse_sensitivity * dtTime;
    pitch += mouse_dir.y * -1.0f * mouse_sensitivity * dtTime;
    glm::fclamp(pitch, -89.0f, 89.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_forward = glm::normalize(direction);
}
}  // namespace Fuego::Graphics
