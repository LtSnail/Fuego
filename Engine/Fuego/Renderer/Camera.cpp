#include "Camera.h"
#include "Input.h"

namespace Fuego::Renderer
{

Camera* Camera::active_camera = nullptr;

Camera::Camera()
{
    view = glm::mat4(1.0f);
    dir = glm::vec3(0.0f, 0.0f, 1.0f);
}

Camera::~Camera()
{
}

void Camera::Update()
{
    KeyCode key = Input::GetPressedKey();
    switch (key)
    {
    case Key::W:
        dir.z = 0.2f;
    case Key::S:
        dir.z = -0.2f;
    case Key::A:
        dir.x = -0.2f;
    case Key::D:
        dir.x = 0.2f;
        
    }

    MoveCamera();
}

void Camera::Activate()
{
    active_camera = this;
}

void Camera::MoveCamera()
{

}

}  // namespace Fuego::Renderer