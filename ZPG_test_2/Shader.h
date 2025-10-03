#pragma once
//Include GLEW
#include <GL/glew.h>


//Include GLFW  
#include <GLFW/glfw3.h>  

#include <vector>

class Shader
{
public:
	GLuint id;
	GLenum type;

	Shader(const char* source, GLenum shaderType);
	~Shader();

	void CheckCompileError();
};

