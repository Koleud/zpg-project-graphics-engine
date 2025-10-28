#include "Camera.h"

Camera::Camera(const glm::vec3& eye, const glm::vec3& center)
{
	this->eye = eye;
	this->center = center;
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);;
}

void Camera::MoveFoward(const glm::vec3& target)
{
	eye += glm::normalize(target) * speed;
	center += glm::normalize(target) * speed;
	Notify();
}

void Camera::MoveBackward(const glm::vec3& target)
{
	eye -= glm::normalize(target) * speed;
	center -= glm::normalize(target) * speed;
	Notify();
}

void Camera::MoveLeft(const glm::vec3& target)
{
	eye -= glm::normalize(glm::cross(target, up)) * speed;
	center -= glm::normalize(glm::cross(target, up)) * speed;
	Notify();
}

void Camera::MoveRight(const glm::vec3& target)
{
	eye += glm::normalize(glm::cross(target, up)) * speed;
	center += glm::normalize(glm::cross(target, up)) * speed;
	Notify();
}

void Camera::Rotate(float yaw, float pitch)
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	center = eye + glm::normalize(direction);
	Notify();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(eye, center, up);
}

glm::vec3 Camera::GetTargetDirection() const
{
	return glm::normalize(center - eye);
}

glm::vec3 Camera::GetPosition() const
{
	return eye;
}

void Camera::UpdateViewMatrix()
{
    viewMatrix = glm::lookAt(eye, center, up);
    Notify();
}

void Camera::UpdateProjection(float aspectRatio)
{
    projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    Notify();
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return projectionMatrix;
}