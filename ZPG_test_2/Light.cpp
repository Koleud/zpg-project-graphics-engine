#include "Light.h"
#include <string>

Light::Light(const glm::vec3& pos, const glm::vec3& col, float intens)
{
	position = pos;
	color = col;
	intensity = intens;
}

void Light::ApplyToShader(ShaderProgram& shaderProgram)
{
	const char* uniformName = "light";
	shaderProgram.SetUniform((std::string(uniformName) + ".position").c_str(), position);
	shaderProgram.SetUniform((std::string(uniformName) + ".color").c_str(), color);
	shaderProgram.SetUniform((std::string(uniformName) + ".intensity").c_str(), intensity);
}
