#include "Camera2D.h"

Camera2D::Camera2D(const glm::vec3& _position, const glm::vec3& _lookAt, const float& left, const float& right, const float& top, const float& bottom, const float& _near, const float& _far)
	: BaseCamera(_position, glm::vec3(0, 1, 0), 60.0f, _near, _far)
{
	this->lookAt = _lookAt;
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
}

glm::mat4 Camera2D::CreateProjectionMatrix() const {
	return glm::ortho(left, right, bottom, top, near, far);
}

void Camera2D::UpdateCamera() {
	view = glm::lookAt(position, lookAt, worldUp);
	projection = glm::ortho(left, right, bottom, top, near, far);
}
