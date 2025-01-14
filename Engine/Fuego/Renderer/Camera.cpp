#include "Camera.h"


namespace Fuego::Renderer
{

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
}


}  // namespace Fuego::Renderer