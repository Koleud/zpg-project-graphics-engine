#include "Transformation.h"

#pragma once
class BezierCurves : public Transformation
{
private:
    glm::vec3 P0, P1, P2, P3;
    float t = 0.0f;           
    float speed = 0.2f;

public:
    BezierCurves(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3, float speed);
    glm::vec3 GetPoint(float t) const;
    void Update() override;
    glm::mat4 GetMatrix() const override;
};

