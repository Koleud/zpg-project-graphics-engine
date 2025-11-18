#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "DrawableObject.h"
#include "ShaderProgram.h"

int DrawableObject::globalID = 1;

DrawableObject::DrawableObject(ShaderProgram* shaderProgram, Model* model)
{
	this->shaderProgram = shaderProgram;
	this->model = model;

    this->id = globalID++;
}

DrawableObject::DrawableObject(ShaderProgram* shaderProgram, Model* model, bool dynamic)
{
    this->shaderProgram = shaderProgram;
    this->model = model;
    this->isDynamic = dynamic;

    this->id = globalID++;
}

DrawableObject::~DrawableObject()
{
}

int DrawableObject::GetID() const
{
    return this->id; 
}

void DrawableObject::Draw()	
{
	shaderProgram->Use();

	glm::mat4 modelMatrix = transform.GetMatrix();
	
	glm::mat4 mvp = modelMatrix;

	shaderProgram->SetUniform("model", mvp);
	model->Draw();
}

