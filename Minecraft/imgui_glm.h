#pragma once
#include "imgui.h"
#include "glm/glm.hpp"

namespace glmToimgui {
	// glm::vec2 -> ImVec2
	inline static ImVec2 Vec2ToImVec2(glm::vec2 vec) {
		return ImVec2(vec.x, vec.y);
	}

	// glm::vec4 -> ImVec4
	inline static ImVec4 Vec4ToImVec4(glm::vec4 vec) {
		return ImVec4(vec.x, vec.y, vec.z, vec.w);
	}

	// ImVec2 -> glm::vec2
	inline static glm::vec2 ImVec2ToVec2(ImVec2 vec) {
		return glm::vec2(vec.x, vec.y);
	}

	// ImVec4 -> glm::vec4
	inline static glm::vec4 ImVec4ToVec4(ImVec4 vec) {
		return glm::vec4(vec.x, vec.y, vec.z, vec.w);
	}
}