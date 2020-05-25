#include "AABB.h"

AABB::AABB(const glm::vec3& origin, const glm::vec3& size) {
	Update(origin, size);
}

AABB::~AABB() {
}

void AABB::Update(const glm::vec3& origin, const glm::vec3 size) {
	this->origin = origin;
	min = origin - (size / 2.0f);
	max = origin + (size / 2.0f);

	points[(int)Corner::TOP_BACK_LEFT] = glm::vec3(min.x, max.y, min.z);
	points[(int)Corner::TOP_BACK_RIGHT]	= glm::vec3(max.x, max.y, min.z);
	points[(int)Corner::TOP_FRONT_LEFT]	= glm::vec3(min.x, max.y, max.z);
	points[(int)Corner::TOP_FRONT_RIGHT] = glm::vec3(max.x, max.y, max.z);

	points[(int)Corner::BOT_BACK_LEFT] = glm::vec3(min.x, min.y, min.z);
	points[(int)Corner::BOT_BACK_RIGHT] = glm::vec3(max.x, min.y, min.z);
	points[(int)Corner::BOT_FRONT_LEFT] = glm::vec3(min.x, min.y, max.z);
	points[(int)Corner::BOT_FRONT_RIGHT] = glm::vec3(max.x, min.y, max.z);
}

bool AABB::IsOverlapping(const glm::vec3& origin_A, const glm::vec3& size_A, const glm::vec3& origin_B, const glm::vec3& size_B) {
	glm::vec3 min_A = origin_A - (size_A / 2.0f);
	glm::vec3 max_A = origin_A + (size_A / 2.0f);

	glm::vec3 min_B = origin_B - (size_B / 2.0f);
	glm::vec3 max_B = origin_B + (size_B / 2.0f);

	if(max_A.x >= min_B.x && min_A.x <= max_B.x && max_A.y >= min_B.y && min_A.y <= max_B.y && max_A.z >= min_B.z && min_A.z <= max_B.z)
		return true;
	return false;
}

bool AABB::IsOverlapping(const glm::vec3& _origin, const glm::vec3& _size) const {
	glm::vec3 min_A = _origin - (_size / 2.0f);
	glm::vec3 max_A = _origin + (_size / 2.0f);

	if(max.x >= min_A.x && min.x <= max_A.x && max.y >= min_A.y && min.y <= max_A.y && max.z >= min_A.z && min.z <= max_A.z)
		return true;
	return false;
}

bool AABB::IsOverlapping(const AABB& aabb) const {
	if(this->max.x >= aabb.min.x && this->min.x <= aabb.max.x && this->max.y >= aabb.min.y && this->min.y <= aabb.max.y && this->max.z >= aabb.min.z && this->min.z <= aabb.max.z)
		return true;
	return false;
}

glm::vec3 AABB::GetPoint(const unsigned int& index) const {
	if(index >= 8) return glm::vec3(0);
	return points[index];
}
