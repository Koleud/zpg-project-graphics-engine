#include "Light.h"
#include <string>

//Ambient light
SceneLight::SceneLight(const glm::vec3& col, float intens)
{
    type = LightType::Ambient;
    position = glm::vec3(0.0f);
    direction = glm::vec3(0.0f);
    color = col;
    intensity = intens;
    range = 0.0f;
    cutoff = 0.0f;
}

//Point light
SceneLight::SceneLight(const glm::vec3& pos, const glm::vec3& col, float intens, float rng)
{
    type = LightType::Point;
    position = pos;
    direction = glm::vec3(0.0f);
    color = col;
    intensity = intens;
    range = rng;
    cutoff = 0.0f;
}

//Directional light
SceneLight::SceneLight(const glm::vec3& dir, const glm::vec3& col, float intens)
{
    type = LightType::Directional;
    position = glm::vec3(0.0f);
    direction = glm::normalize(dir);
    color = col;
    intensity = intens;
    range = 0.0f;
    cutoff = 0.0f;
}

//Spotlight
SceneLight::SceneLight(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& col, float intens, float rng, float angle)
{
    type = LightType::Spotlight;
    position = pos;
    direction = glm::normalize(dir);
    color = col;
    intensity = intens;
    range = rng;
    cutoff = angle;
}

glm::vec3 SceneLight::GetWorldPosition() const 
{
    glm::mat4 model = transform.GetMatrix();
    return glm::vec3(model * glm::vec4(position, 1.0f));
}
