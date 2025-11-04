#include <glm/vec3.hpp>

#pragma once
class Material
{
public:
    glm::vec3 ambient;   // ra
    glm::vec3 diffuse;   // rd
    glm::vec3 specular;  // rs
    float shininess;     // h

    Material(const glm::vec3& a, const glm::vec3& d, const glm::vec3& s, float sh);
};

