#include "DynamicTranslate.h"

DynamicTranslate::DynamicTranslate(const glm::vec3& startPos, const glm::vec3& vel)
{
    startPosition = startPos;
    velocity = vel;
    currentPosition = startPos;
}

void DynamicTranslate::Update(float deltaTime)
{
    currentPosition += velocity * deltaTime;
}

glm::mat4 DynamicTranslate::GetMatrix() const
{
    return glm::translate(glm::mat4(1.0f), currentPosition);
}
