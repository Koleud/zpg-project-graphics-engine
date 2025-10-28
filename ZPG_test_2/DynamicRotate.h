#include "Transformation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma once
class DynamicRotate : public Transformation
{
private:
    glm::vec3 axis;      // ось вращения
    float speed;         // скорость (в радианах в секунду)
    float angle;         // текущий угол

public:
    DynamicRotate(const glm::vec3& axis, float speed);
    void Update(float deltaTime) override;
    glm::mat4 GetMatrix() const override;
};

