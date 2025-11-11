#pragma once
//Include GLEW
#include <GL/glew.h>


//Include GLFW  
#include <GLFW/glfw3.h>  

#include <vector>

class Model
{
private:
	GLuint VBO = 0; GLuint VAO = 0;
    int vertexCount;

public:
	Model(const std::vector<float>& points, int vertexCount);
    Model(const std::vector<float>& points, int vertexCount, bool hasUV);
    Model(const std::string& objPath, const std::string& mtlPath);

	void Draw();
	~Model();
};

