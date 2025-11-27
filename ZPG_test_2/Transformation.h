#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma once
class Transformation
{
public:
    float deltaTime = 1.0f;
	virtual glm::mat4 GetMatrix() const = 0;
	virtual ~Transformation() = default;
    virtual void Update() {};
};

