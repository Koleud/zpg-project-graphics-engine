#include "Transformation.h"

#pragma once
class Scale : public Transformation
{
private:
	glm::vec3 scale; // Scaling factors for x, y, z axes

public:
	Scale(const glm::vec3& scale);
	glm::mat4 GetMatrix() const override;
};

