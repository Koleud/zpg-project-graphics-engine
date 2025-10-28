#pragma once
//Include GLEW
#include <GL/glew.h>


//Include GLFW  
#include <GLFW/glfw3.h>  

//Include GLM
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <vector>
#include "Shader.h"
#include "Observer.h"
#include "Camera.h"
#include "Light.h"

class ShaderProgram : public Observer
{
private:
	GLuint program_id = 0;;
	GLuint VBO = 0; GLuint VAO = 0;
	int vertexCount;
	Camera* camera;

public:
	ShaderProgram(const Shader& vertex_shader, const Shader& fragment_shader, Camera* cam);
	~ShaderProgram();

	void Update() override;

	void Use();
	//void Draw();
	void CheckCompileError();
	void SetUniform(const char* name, const glm::mat4& matrix);
	void SetUniform(const char* name, const float value);
	void SetUniform(const char* name, const int value);
	void SetUniform(const char* name, const glm::vec3& vector);
	void SetUniform(const char* name, const glm::vec4& vector);
	void SetUniform(const char* name, const glm::mat3& matrix);

    void SetLight(const std::vector<SceneLight>& lights);
};

