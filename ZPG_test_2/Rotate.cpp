#include "Rotate.h"

Rotate::Rotate(const glm::vec3& rotation)
{
	this->rotation = rotation;
    this->pivot = glm::vec3(0.0f, 0.0f, 0.0f);
}

Rotate::Rotate(const glm::vec3& rotation, const glm::vec3& pivot)
{
    this->rotation = rotation;
    this->pivot = pivot;
}

glm::mat4 Rotate::GetMatrix() const
{
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    rotationMatrix = glm::translate(rotationMatrix, pivot);

    rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    rotationMatrix = glm::translate(rotationMatrix, -pivot);

    return rotationMatrix;
}



