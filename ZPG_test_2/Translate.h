#include "Transformation.h"

#pragma once
class Translate : public Transformation
{
private:
	glm::vec3 translation;

public:

	Translate(const glm::vec3& translation);

	glm::mat4 GetMatrix() const override;
};

