#include "BezierSpline.h"

glm::vec3 BezierSpline::EvaluatePosition(int seg, float t) const
{
    int i = segmentStarts[seg];

    const auto& P0 = points[i];
    const auto& P1 = points[i + 1];
    const auto& P2 = points[i + 2];
    const auto& P3 = points[i + 3];

    float u = 1.0f - t;

    return u * u * u * P0 +
        3 * u * u * t * P1 +
        3 * u * t * t * P2 +
        t * t * t * P3;
}

glm::vec3 BezierSpline::EvaluateTangent(int seg, float t) const
{
    int i = segmentStarts[seg];

    const auto& P0 = points[i];
    const auto& P1 = points[i + 1];
    const auto& P2 = points[i + 2];
    const auto& P3 = points[i + 3];

    float u = 1.0f - t;

    glm::vec3 tangent =
        3.0f * u * u * (P1 - P0) +
        6.0f * u * t * (P2 - P1) +
        3.0f * t * t * (P3 - P2);

    return glm::normalize(tangent);
}

void BezierSpline::AddPoint(const glm::vec3& p)
{
    points.push_back(p);
}

void BezierSpline::FinalizeSegments()
{
    segmentStarts.clear();
    for (int i = 0; i + 3 < points.size(); i += 3)
        segmentStarts.push_back(i);
}

glm::mat4 BezierSpline::GetMatrix() const
{
    glm::vec3 pos = EvaluatePosition(currentSegment, t);
    glm::vec3 forward = EvaluateTangent(currentSegment, t);
    glm::vec3 worldUp(0, 1, 0);

    glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));
    glm::vec3 up = glm::cross(right, forward);

    glm::mat4 R(1.0f);
    R[0] = glm::vec4(-forward, 0);
    R[1] = glm::vec4(up, 0);
    R[2] = glm::vec4(right, 0);

    glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
    return T * R;
}

void BezierSpline::Update()
{
    t += speed * deltaTime;

    if (t > 1.0f)
    {
        t -= 1.0f;
        currentSegment++;

        if (currentSegment >= segmentStarts.size())
            currentSegment = 0;
    }
}