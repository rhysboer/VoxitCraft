#pragma once
#include "glm/glm.hpp"

class AABB {
public:

	enum class Corner {
		TOP_BACK_LEFT,
		TOP_BACK_RIGHT,
		TOP_FRONT_LEFT,
		TOP_FRONT_RIGHT,

		BOT_BACK_LEFT,
		BOT_BACK_RIGHT,
		BOT_FRONT_LEFT,
		BOT_FRONT_RIGHT
	};

	AABB(const glm::vec3& origin, const glm::vec3& size);
	~AABB();

	void Update(const glm::vec3& origin, const glm::vec3 size);

	static bool IsOverlapping(const glm::vec3& origin_A, const glm::vec3& size_A, const glm::vec3& origin_B, const glm::vec3& size_B);
	bool IsOverlapping(const glm::vec3& _origin, const glm::vec3& _size) const;
	bool IsOverlapping(const AABB& aabb) const;

	// Will return vec3(0) if index is over 8
	glm::vec3 GetPoint(const unsigned int& index) const;

private:

	glm::vec3 points[8];

	glm::vec3 origin;
	glm::vec3 min;
	glm::vec3 max;
};

