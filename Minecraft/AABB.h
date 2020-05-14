#pragma once
#include "glm/glm.hpp"

class AABB {
public:

	static bool IsOverlapping(const glm::vec3& origin_A, const glm::vec3& size_A, const glm::vec3& origin_B, const glm::vec3& size_B);
	bool IsOverlapping(const glm::vec3& origin_A, const glm::vec3& size_A) const;
	bool IsOverlapping(const AABB& aabb) const;

private:

};

