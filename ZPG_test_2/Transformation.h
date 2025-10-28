#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma once
class Transformation
{
public:
	virtual glm::mat4 GetMatrix() const = 0;
	virtual ~Transformation() = default;
    virtual void Update(float deltaTime) {};
};

