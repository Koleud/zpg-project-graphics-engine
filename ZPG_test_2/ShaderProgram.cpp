#include "ShaderProgram.h"
//Include GLEW
#include <GL/glew.h>


//Include GLFW  
#include <GLFW/glfw3.h>  

#include <vector>
#include "ShaderProgram.h"
#include "Shader.h"

ShaderProgram::ShaderProgram(const std::vector<float>& points, const Shader& vertex_shader, const Shader& fragment_shader, int vertexCount)
{
	program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader.id);
	glAttachShader(program_id, fragment_shader.id);
	glLinkProgram(program_id);

	CheckCompileError();

	this->vertexCount = vertexCount;
	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);

	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(program_id);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void ShaderProgram::Use()
{
	glUseProgram(program_id);
}
void ShaderProgram::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
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
