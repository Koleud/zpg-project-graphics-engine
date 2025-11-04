#include "ShaderProgram.h"
//Include GLEW
#include <GL/glew.h>


//Include GLFW  
#include <GLFW/glfw3.h>  

#include <vector>
#include "ShaderProgram.h"
#include "Shader.h"

ShaderProgram::ShaderProgram(const Shader& vertex_shader, const Shader& fragment_shader, Camera* cam)
{
	camera = cam;

	program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader.id);
	glAttachShader(program_id, fragment_shader.id);
	glLinkProgram(program_id);

	CheckCompileError();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(program_id);
}

void ShaderProgram::Update()
{
	glm::mat4 view = camera->GetViewMatrix();
	glm::vec3 camPos = camera->GetPosition();
    glm::mat4 projection = camera->GetProjectionMatrix();

	Use();
	SetUniform("view", view);
	SetUniform("cameraPosition", camPos);
    SetUniform("projection", projection);
}

void ShaderProgram::Use()
{
	glUseProgram(program_id);
}

void ShaderProgram::CheckCompileError()
{
	GLint status;
	glGetShaderiv(program_id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(program_id, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(program_id, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Compile failure in shader:\n%s\n", strInfoLog);
		delete[] strInfoLog;
	}
}

void ShaderProgram::SetUniform(const char* name, const glm::mat4& matrix)
{
	GLuint loc = glGetUniformLocation(program_id, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::SetUniform(const char* name, const float value)
{
	GLuint loc = glGetUniformLocation(program_id, name);
	glUniform1f(loc, value);
}

void ShaderProgram::SetUniform(const char* name, const int value)
{
	GLuint loc = glGetUniformLocation(program_id, name);
	glUniform1i(loc, value);
}

void ShaderProgram::SetUniform(const char* name, const glm::vec3& vector)
{
	GLuint loc = glGetUniformLocation(program_id, name);
	glUniform3fv(loc, 1, glm::value_ptr(vector));
}

void ShaderProgram::SetUniform(const char* name, const glm::vec4& vector)
{
	GLuint loc = glGetUniformLocation(program_id, name);
	glUniform4fv(loc, 1, glm::value_ptr(vector));
}

void ShaderProgram::SetUniform(const char* name, const glm::mat3& matrix)
{
	GLuint loc = glGetUniformLocation(program_id, name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::SetLight(const std::vector<SceneLight>& lights)
{
    int lightCount = static_cast<int>(lights.size());
    SetUniform("lightCount", lightCount);

    for (int i = 0; i < lightCount; i++)
    {
        const SceneLight& l = lights[i];
        std::string prefix = "lights[" + std::to_string(i) + "]";

        SetUniform((prefix + ".position").c_str(), l.GetWorldPosition());
        SetUniform((prefix + ".color").c_str(), l.color);
        SetUniform((prefix + ".intensity").c_str(), l.intensity);
        SetUniform((prefix + ".range").c_str(), l.range);
        SetUniform((prefix + ".cutoff").c_str(), l.cutoff);

        SetUniform((prefix + ".type").c_str(), static_cast<int>(l.type));

        if (l.type == SceneLight::LightType::Directional || l.type == SceneLight::LightType::Spotlight)
        {
            SetUniform((prefix + ".direction").c_str(), glm::normalize(l.direction));
        }
    }
}