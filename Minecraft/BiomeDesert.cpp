#include "BiomeDesert.h"

BiomeDesert::BiomeDesert() {
	name = "Desert";
	genParameters.structureDensity = 600;
	genParameters.scale = 260.0f;//700.0f;
	genParameters.yOffset = 10.0f;
}

BiomeDesert::~BiomeDesert() {
}

BlockIDs BiomeDesert::GetBlock(const int& height) const {
	if(height <= 0)
		return BlockIDs::SAND;
	return BlockIDs::AIR;
}

/*
void BiomeDesert::AddStructure(Chunk& chunk, const glm::vec3& position) const {
	const int height = (rand() % 4) + 3;

	for(int i = 0; i < height; i++) {
		chunk.SetBlock(position.x, position.y + i, position.z, BlockIDs::CACTUS);
	}	
}
*/
