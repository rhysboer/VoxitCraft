#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Time.h"
#include "Input.h"

#define CAMERA_SENSITIVITY 0.3f
#define CAMERA_SPEED 30.0f
#define CAMERA_BOOST 6.0f

#define DIRTY_CHECK if(isDirty) UpdateCamera();

class Engine;
class Frustum;
class Camera {
public:
	Camera(float pos_x, float pos_y, float pos_z, float worldUp_x, float worldUp_y, float worldUp_z, float yaw, float pitch, float fov, float near, float far);
	Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, float fov, float near, float far);
	Camera(glm::vec3 position, glm::vec3 worldUp = glm::vec3(0, 1, 0), float near = 0.1f, float far = 1000.0f);
	~Camera();

	void InputHandler();
	void Rotate(float x, float y);
	void SetFar(const float& value);
	void SetDirty();

	glm::mat4 View();
	glm::mat4 Projection();
	glm::mat4 ProjectionView();

	glm::vec3 Up();
	glm::vec3 Right();
	glm::vec3 Forward();

	glm::vec3 Position();
	Frustum* GetFrustum() const;
	float GetFar() const;
	float GetNear() const;
	bool IsDirty() const;

	static Camera* ActiveCamera;

	friend class Frustum;
private:

	void UpdateCamera();
	void LimitRotation();

	Frustum* frustum;

	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	float fov;
	float _near;
	float _far;

	bool isDirty;
};

class Frustum {
public:
	std::vector<glm::vec3> points = std::vector<glm::vec3>();
	glm::vec3 center = glm::vec3(0.0f);

	friend class Camera;
private:
	void UpdateFrustum(const Camera& camera);
};
