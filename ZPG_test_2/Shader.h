#pragma once
//Include GLEW
#include <GL/glew.h>


//Include GLFW  
#include <GLFW/glfw3.h>  

#include <vector>

class Shader
{
private:
	GLuint VBO = 0;
	GLuint VAO = 0;

	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	GLuint shaderProgram = 0;

	int vertexCount = 0;
public:

	Shader(const std::vector<float>& points, const char* vertex_shader, const char* fragment_shader, int vertexCount);
	~Shader();

	void Use();
	void Draw();
	void CheckCompileError();
};

