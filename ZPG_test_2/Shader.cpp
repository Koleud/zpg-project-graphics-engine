#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include "Shader.h"

Shader::Shader(const char* source, GLenum shaderType)
{
	type = shaderType;
	id = glCreateShader(type);
	glShaderSource(id, 1, &source, NULL);
	glCompileShader(id);

	//kontrola kompilace shaderu
    CheckCompileError();
}

Shader::Shader(const std::string source, GLenum shaderType)
{
    type = shaderType;
    id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    //kontrola kompilace shaderu
    CheckCompileError();
}

Shader::~Shader()
{
	glDeleteShader(id);
}

void Shader::CheckCompileError() {
    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::string infoLog(length, ' ');
        glGetShaderInfoLog(id, length, NULL, &infoLog[0]);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
    }
}
