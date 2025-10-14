#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>

#include "ShaderProgram.h"
#include "Model.h"
#include "CompositeTransformation.h"
class DrawableObject
{
private:
	ShaderProgram* shaderProgram;
	Model* model;

public:
	CompositeTransformation transform;
	DrawableObject(ShaderProgram* shaderProgram, Model* model);
	~DrawableObject();

	void Draw();
};

