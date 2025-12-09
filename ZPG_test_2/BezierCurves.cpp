#include "BezierCurves.h"

BezierCurves::BezierCurves(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3, float speed)
{
    this->P0 = P0;
    this->P1 = P1;
    this->P2 = P2;
    this->P3 = P3;
    this->speed = speed;
}

glm::vec3 BezierCurves::GetPoint(float t) const
{
    return (glm::vec3(0.0f, 0.0f, 0.0f));
}

void BezierCurves::Update()
{
    t += deltaTime * speed;
    if (t > 1.0f)
    {
        t = 1.0f;
    }
}

glm::mat4 BezierCurves::GetMatrix() const
{
    float u = 1.0f - t;

    //Position on curve 
    glm::vec3 pos =
        u * u * u * P0 +
        3 * u * u * t * P1 +
        3 * u * t * t * P2 +
        t * t * t * P3;

    //Tangent
    glm::vec3 tangent =
        3.0f * u * u * (P1 - P0) +
        6.0f * u * t * (P2 - P1) +
        3.0f * t * t * (P3 - P2);

    glm::vec3 forward = glm::normalize(tangent);

    //Orientation 
    glm::vec3 worldUp(0, 1, 0);
    glm::vec3 right = glm::normalize(glm::cross(worldUp, forward));
    glm::vec3 up = glm::cross(forward, right);

    glm::mat4 R(1.0f);
    R[0] = glm::vec4(-forward, 0.0f);
    R[1] = glm::vec4(up, 0.0f);
    R[2] = glm::vec4(right, 0.0f);

    //Translation
    glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);

    return T * R;
}
