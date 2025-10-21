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
	Use();
	SetUniform("view", view);
	SetUniform("cameraPosition", camPos);
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