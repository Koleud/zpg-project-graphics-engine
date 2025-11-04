#include <glm/glm.hpp>
#include "CompositeTransformation.h"

#pragma once
class SceneLight
{
private:

public:

    enum class LightType {
        Ambient = 0,
        Point = 1,
        Directional = 2,
        Spotlight = 3
    };


    LightType type;
    glm::vec3 position;     // for Point and Spotlight
    glm::vec3 direction;    // for Directional and Spotlight
    glm::vec3 color;
    float intensity;
    float range;
    float cutoff;           // angle for (Spotlight)

    CompositeTransformation transform;

    SceneLight(const glm::vec3& col, float intens); // ambient
    SceneLight(const glm::vec3& pos, const glm::vec3& col, float intens, float rng); // point (we used it)
    SceneLight(const glm::vec3& dir, const glm::vec3& col, float intens); // directional
    SceneLight(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& col, float intens, float rng, float cutoffAngle); // spotlight


    glm::vec3 GetWorldPosition() const;
};

