#include "Transformation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma once
class DynamicRotate : public Transformation
{
private:
    glm::vec3 axis;
    float speed;         // rotation speed in radians
    float angle;         // current angle in radians

public:
    DynamicRotate(const glm::vec3& axis, float speed);
    void Update() override;
    glm::mat4 GetMatrix() const override;
};

