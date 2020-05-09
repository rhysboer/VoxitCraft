#pragma once
#include "glm/glm.hpp"
#include "BaseCamera.h"
#include "BlockManager.h"

namespace Ray {
	#define DOUBLE_MAX 1.79769e+308

	struct RayHit {
		RayHit(BlockIDs hitType, const glm::vec3& hitPosition, BlockIDs neighbourType, const glm::vec3 neighbourPosition, bool hit) :
		hitBlockType(hitType), hitPosition(hitPosition), neighbourBlockType(neighbourType), neighbourPosition(neighbourPosition), hit(hit) {}

		BlockIDs hitBlockType;
		BlockIDs neighbourBlockType;
		
		glm::vec3 hitPosition;
		glm::vec3 neighbourPosition;

		bool hit;
	};

	RayHit RaycastFromCamera(const float& distance, BaseCamera& camera);
}

