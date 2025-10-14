#include "Transformation.h"

#pragma once
class Rotate : public Transformation
{
private:
	glm::vec3 rotation; // Rotation angles in radians for x, y, z axes

public:
	Rotate(const glm::vec3& rotation);
	glm::mat4 GetMatrix() const override;
};

