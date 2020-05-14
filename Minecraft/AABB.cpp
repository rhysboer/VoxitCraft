#include "AABB.h"

bool AABB::IsOverlapping(const glm::vec3& origin_A, const glm::vec3& size_A, const glm::vec3& origin_B, const glm::vec3& size_B) {
	glm::vec3 min_A = origin_A - (size_A / 2.0f);
	glm::vec3 max_A = origin_A + (size_A / 2.0f);

	glm::vec3 min_B = origin_B - (size_B / 2.0f);
	glm::vec3 max_B = origin_B + (size_B / 2.0f);

	if(max_A.x >= min_B.x && min_A.x <= max_B.x && max_A.y >= min_B.y && min_A.y <= max_B.y && max_A.z >= min_B.z && min_A.z <= max_B.z)
		return true;
	return false;
}
