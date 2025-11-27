#include "Transformation.h"
#include <vector>

#pragma once
class CompositeTransformation : public Transformation
{
private:
	std::vector<Transformation*> transformations; // Vector of pointers to Transformation objects
public:
	CompositeTransformation();
	void AddTransformation(Transformation* transformation);

	glm::mat4 GetMatrix() const override;

	void UpdateTransformation();

    glm::vec3 GetPosition() const;


    void ClearTransformations();
};

