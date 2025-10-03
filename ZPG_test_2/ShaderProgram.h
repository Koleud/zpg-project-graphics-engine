#pragma once
//Include GLEW
#include <GL/glew.h>


//Include GLFW  
#include <GLFW/glfw3.h>  

#include <vector>
#include "Shader.h"

class ShaderProgram
{
private:
	GLuint program_id = 0;;
	GLuint VBO = 0; GLuint VAO = 0;
	int vertexCount;

public:
	ShaderProgram(const std::vector<float>& points, const Shader& vertex_shader, const Shader& fragment_shader, int vertexCount);
	~ShaderProgram();

	void Use();
	void Draw();
	void CheckCompileError();
};

