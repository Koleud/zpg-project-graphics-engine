#include "Transformation.h"
#include <vector>

#pragma once
class BezierSpline : public Transformation
{
private:
    std::vector<glm::vec3> points;
    std::vector<int> segmentStarts;  // P0 indices for each segment

    float t = 0.0f;
    float speed = 0.001f;
    int currentSegment = 0;

    glm::vec3 EvaluatePosition(int seg, float t) const;
    glm::vec3 EvaluateTangent(int seg, float t) const;

public:
    BezierSpline() = default;

    void AddPoint(const glm::vec3& p);
    void FinalizeSegments();

    glm::mat4 GetMatrix() const override;
    void Update() override;

};

