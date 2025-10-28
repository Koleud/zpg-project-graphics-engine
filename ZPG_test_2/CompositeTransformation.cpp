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

void CompositeTransformation::UpdateTransformation(float deltaTime)
{
    for (auto* t : transformations)
        t->Update(deltaTime);
}