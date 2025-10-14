#include "Translate.h"

Translate::Translate(const glm::vec3& translation)
{
	this->translation = translation;
}

glm::mat4 Translate::GetMatrix() const
{
	return glm::translate(glm::mat4(1.0f), translation);
}
