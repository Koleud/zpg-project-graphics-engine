#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "DrawableObject.h"
#include "ShaderProgram.h"

DrawableObject::DrawableObject(ShaderProgram* shaderProgram, Model* model)
{
	this->shaderProgram = shaderProgram;
	this->model = model;
}

DrawableObject::~DrawableObject()
{
}

void DrawableObject::Draw()	
{
	shaderProgram->Use();

	glm::mat4 modelMatrix = transform.GetMatrix();
	
	glm::mat4 mvp = modelMatrix;

	shaderProgram->SetUniform("model", mvp);
	model->Draw();
}

