#include "Scale.h"

Scale::Scale(const glm::vec3& scale)
{
	this->scale = scale;
}

glm::mat4 Scale::GetMatrix() const
{
	return glm::scale(glm::mat4(1.0f), scale);
}
