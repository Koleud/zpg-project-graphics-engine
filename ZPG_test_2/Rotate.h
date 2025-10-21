#include "Transformation.h"

#pragma once
class Rotate : public Transformation
{
private:
	glm::vec3 rotation; // Rotation angles in radians for x, y, z axes
    glm::vec3 pivot;   // Pivot point for rotation

public:
	Rotate(const glm::vec3& rotation);
    Rotate(const glm::vec3& rotation, const glm::vec3& pivot);
	glm::mat4 GetMatrix() const override;
};

