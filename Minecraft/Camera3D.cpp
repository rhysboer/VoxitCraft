#include "Camera3D.h"
#include "Engine.h"

Camera3D::Camera3D(const glm::vec3& _position, const glm::vec3& _worldUp, const float& _fov, const float& _near, const float& _far) : BaseCamera(_position, _worldUp, _fov, _near, _far)  { 
}

glm::mat4 Camera3D::CreateProjectionMatrix() const {
	glm::vec2 size = Engine::GetWindowSize();
	return 	glm::perspective(fov, (size.y == 0) ? 0.0f : size.x / size.y, near, far);
}
