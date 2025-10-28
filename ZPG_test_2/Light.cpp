#include "Light.h"
#include <string>

SceneLight::SceneLight(const glm::vec3& pos, const glm::vec3& col, float intens)
{
	position = pos;
	color = col;
	intensity = intens;
}

glm::vec3 SceneLight::GetWorldPosition() const 
{
    glm::mat4 model = transform.GetMatrix();
    return glm::vec3(model * glm::vec4(position, 1.0f));
}

//void Light::ApplyToShader(ShaderProgram& shaderProgram)
//{
//	const char* uniformName = "light";
//	shaderProgram.SetUniform((std::string(uniformName) + ".position").c_str(), position);
//	shaderProgram.SetUniform((std::string(uniformName) + ".color").c_str(), color);
//	shaderProgram.SetUniform((std::string(uniformName) + ".intensity").c_str(), intensity);
//}
