#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Transform {
public:
	Transform();
	~Transform();

	void SetPosition(const glm::vec3& position);
	void SetScale(const glm::vec3& scale);
	void SetScale(const float& scale);
	void SetRotation(const glm::vec3& euler);
	void SetRotation(const glm::quat& rotation);

	// Rotate
	void RotateX(const float& angle);
	void RotateY(const float& angle);
	void RotateZ(const float& angle);
	void RotateAroundPoint(const glm::vec3& point, const float& angle, const glm::vec3& axis);

	void SetParent(Transform* parent);

	void Translate(const glm::vec3& direction);
	void Translate(const float& x, const float& y, const float& z);

	glm::vec3 GetPosition() const;
	glm::mat4 GetMatrix();

	void SetDirty();

private:
	void UpdateTransform();

	bool isDirty;

	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	glm::mat4 transform;
	Transform* parent;
};

