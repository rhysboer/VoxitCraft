#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define DIRTY_CHECK if(isDirty) UpdateCamera();

class Engine;

class BaseCamera {
public:

	BaseCamera(const glm::vec3& _position, const glm::vec3& _worldUp = glm::vec3(0.0f, 1.0f, 0.0f), const float& _near = 0.1f, const float& _far = 1000.0f);
	virtual ~BaseCamera() {};

	void Rotate(const float& x, const float& y, const float& strength);

	void SetNear(const float& distance);
	void SetFar(const float& distance);
	void SetFOV(const float& fov);
	void SetDirty();

	glm::mat4 View();
	glm::mat4 Projection();
	glm::mat4 ProjectionView();

	glm::vec3 Up();
	glm::vec3 Right();
	glm::vec3 Forward();

	glm::vec3 GetPosition() const;
	float GetNear() const;
	float GetFar() const;

protected:

	void UpdateCamera();

	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw, pitch, fov, near, far;

	bool isDirty;
};

