#include "Camera.h"

#include "Input.h"

namespace Fuego::Renderer
{

Camera* Camera::active_camera = nullptr;

Camera::Camera()
    : speed(0.1)
    , position(0.0f)
{
    view = glm::mat4(1.0f);
    dir = glm::vec3(0.0f, 0.0f, 1.0f);
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
    dir = glm::vec3(0.0f);

    if (Input::IsKeyPressed(Key::W))
    {
        dir.z = 1.0f * speed;
    }
    else if (Input::IsKeyPressed(Key::S))
    {
        dir.z = -1.0f * speed;
    }
    if (Input::IsKeyPressed(Key::A))
    {
        dir.x = 1.0f * speed;
    }
    else if (Input::IsKeyPressed(Key::D))
    {
        dir.x = -1.0f * speed;
    }
    TranslateCamera();
    RotateCamera();
}

void Camera::TranslateCamera()
{
    position += dir;
    view = glm::translate(glm::mat4(1.0f), position);
}

void Camera::RotateCamera()
{
    glm::vec2 mouseDir = Input::GetMouseDir();
    float angle = glm::atan(mouseDir.y, mouseDir.x);
    glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    view = glm::rotate(view, angle, rotationAxis);
}

}  // namespace Fuego::Renderer
