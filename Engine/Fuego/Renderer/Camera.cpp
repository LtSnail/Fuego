#include "Camera.h"

#include "Input.h"

namespace Fuego::Renderer
{

Camera* Camera::active_camera = nullptr;

Camera::Camera()
    : speed(0.1)
    , position(0.0f)
    , up(glm::vec3(0.0f, 1.0f, 0.0f))
    , yaw(0.0f)
    , pitch(0.0f)
    , mouse_sensitivity(1.0f)
    , camera_forward(0.0f, 0.0f, -1.0f)
    , view(glm::mat4(1.0f))
    , dir(glm::vec3(0.0f, 0.0f, 0.0f))
    , projection(glm::mat4(1.0f))
    , FOV(60)
    , near_clip(0.1f)
    , far_clip(1000.0f)

{
}

Camera::~Camera()
{
}

void Camera::Activate()
{
    active_camera = this;
}

void Camera::Update()
{
    if (Input::IsKeyPressed(Key::W))
    {
        position += speed * camera_forward;
    }
    if (Input::IsKeyPressed(Key::S))
    {
        position -= speed * camera_forward;
    }
    if (Input::IsKeyPressed(Key::A))
    {
        position -= glm::normalize(glm::cross(camera_forward, up)) * speed;
    }
    if (Input::IsKeyPressed(Key::D))
    {
        position += glm::normalize(glm::cross(camera_forward, up)) * speed;
    }

    RotateCamera();
    projection = glm::perspective(glm::radians((float)FOV), 1280.0F / 720.0F, near_clip, far_clip);
    view = glm::lookAt(position, position + camera_forward, up);
}

void Camera::RotateCamera()
{
    glm::vec2 mouse_dir = Input::GetMouseDir();
    yaw += mouse_dir.x * mouse_sensitivity;
    pitch += mouse_dir.y * -1.0f * mouse_sensitivity;
    glm::fclamp(pitch, -89.0f, 89.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_forward = glm::normalize(direction);
}
}  // namespace Fuego::Renderer
