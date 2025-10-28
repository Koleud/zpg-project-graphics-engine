#include <glm/glm.hpp>
#include "CompositeTransformation.h"

#pragma once
class SceneLight
{
private:

public:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;

    CompositeTransformation transform;

    SceneLight(const glm::vec3& pos, const glm::vec3& col, float intens);

    glm::vec3 GetWorldPosition() const;

	//void ApplyToShader(ShaderProgram& shader);
};

