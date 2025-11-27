#include "DynamicRotate.h"

DynamicRotate::DynamicRotate(const glm::vec3& axis, float speed)
{
    this->axis = glm::normalize(axis);
    this->speed = speed;
    this->angle = 0.0f;
}

void DynamicRotate::Update()
{
    angle += speed * deltaTime;
    if (angle > glm::two_pi<float>())   // angle > 180 degrees
        angle -= glm::two_pi<float>();
}

glm::mat4 DynamicRotate::GetMatrix() const
{
    return glm::rotate(glm::mat4(1.0f), angle, axis);
}
