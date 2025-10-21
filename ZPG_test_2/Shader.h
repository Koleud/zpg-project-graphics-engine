#pragma once
//Include GLEW
#include <GL/glew.h>


//Include GLFW  
#include <GLFW/glfw3.h>  

#include <vector>
#include <string>

class Shader
{
public:
	GLuint id;
	GLenum type;

	Shader(const char* source, GLenum shaderType);
    Shader(const std::string source, GLenum shaderType);
	~Shader();

	void CheckCompileError();
};

