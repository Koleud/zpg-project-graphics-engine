#include <glm/glm.hpp>
#include "ShaderProgram.h"

#pragma once
class Light
{
private:
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
public:
	Light(const glm::vec3& pos, const glm::vec3& col, float intens);

	void ApplyToShader(ShaderProgram& shader);
};

