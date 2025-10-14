#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Subject.h"


#pragma once
class Camera : public Subject
{
private:
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;
	float speed = 0.05f;
public:
	Camera(const glm::vec3& eye, const glm::vec3& center);

	void MoveFoward(const glm::vec3& target);
	void MoveBackward(const glm::vec3& target);
	void MoveLeft(const glm::vec3& target);
	void MoveRight(const glm::vec3& target);

	void Rotate(float yaw, float pitch);

	glm::mat4 GetViewMatrix() const;
	glm::vec3 GetTargetDirection() const;
};

