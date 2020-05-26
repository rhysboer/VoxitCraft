#include "BiomeSnow.h"

BiomeSnow::BiomeSnow() {
}

BiomeSnow::~BiomeSnow() {
}

BlockIDs BiomeSnow::GetBlock(const int& height) const {
	if(height <= 0)
		return BlockIDs::SNOW_DIRT;
	return BlockIDs::AIR;
}

/*
void BiomeSnow::AddStructure(Chunk& chunk, const glm::vec3& position) const {
}
*/
