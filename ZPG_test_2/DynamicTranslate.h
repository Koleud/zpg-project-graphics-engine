#include "Transformation.h"
#include <glm/glm.hpp>

#pragma once
class DynamicTranslate : public Transformation
{
private:
    glm::vec3 startPosition;
    glm::vec3 velocity;
    glm::vec3 currentPosition;

public:

    DynamicTranslate(const glm::vec3& startPos, const glm::vec3& vel);

    void Update() override;

    glm::mat4 GetMatrix() const override;
};

