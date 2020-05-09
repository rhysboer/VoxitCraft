#pragma once
#include "BaseCamera.h"

class Camera3D : public BaseCamera {
public:

	Camera3D(const glm::vec3& _position, const glm::vec3& _worldUp = glm::vec3(0.0f, 1.0f, 0.0f), const float& _fov = 60, const float& _near = 0.1f, const float& _far = 1000.0f);

protected:

	virtual glm::mat4 CreateProjectionMatrix() const override;
};

