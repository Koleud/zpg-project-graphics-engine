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

//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>
#include <vector>

//Include the shader class
#include "Shader.h"
#include "ShaderProgram.h"
#include "Application.h"
#include "Model.h"
#include "DrawableObject.h"
#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::AddObject(DrawableObject* object)
{
	drawObjects.push_back(object);
}

DrawableObject* Scene::GetObject(int index)
{
	if (index < 0 || index >= drawObjects.size())
		return nullptr;
	return drawObjects[index];
}

void Scene::DrawAll()
{
	for (auto obj : drawObjects)
	{
		obj->Draw();
	}
}
