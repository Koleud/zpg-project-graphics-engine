#include "CompositeTransformation.h"

CompositeTransformation::CompositeTransformation() = default;

void CompositeTransformation::AddTransformation(Transformation* transformation) 
{
	transformations.push_back(transformation);
}

glm::mat4 CompositeTransformation::GetMatrix() const
{
	glm::mat4 result = glm::mat4(1.0f);
	for (const auto& transformation : transformations)
	{
		result = transformation->GetMatrix() * result;
	}
	return result;
}

void CompositeTransformation::UpdateTransformation(int index, Transformation* newTrans)
{
	if (index >= 0 && index < transformations.size()) 
	{
		delete transformations[index];
		transformations[index] = newTrans;
	}
}