#include "BiomeField.h"

BiomeField::BiomeField() {
	name = "Field";
	genParameters.scale = 260.0f;
	genParameters.yOffset = 10.0f;
}

BiomeField::~BiomeField() {
}

BlockIDs BiomeField::GetBlock(const int& height) const {
	if(height > 0)
		return BlockIDs::AIR;
	if(height == 0)
		return BlockIDs::GRASS;
	if(height > -3)
		return BlockIDs::DIRT;
	if(height > -100)
		return BlockIDs::STONE;

	return BlockIDs::AIR;
}

void BiomeField::AddStructure(Chunk& chunk, const glm::vec3& position) const {
	const int height = (rand() % 4) + 6;

	for(int i = 0; i < height; i++) {
		chunk.SetBlock(position.x, position.y + i, position.z, BlockIDs::WOOD);
	}

	for(int y = 0; y < 6; y++) {
		for(int z = 0; z < 7; z++) {
			for(int x = 0; x < 7; x++) {
				if(TREE_STRUCTURE[y][z][x] == 1)
					chunk.SetBlock(position.x + x - 3, position.y + y + height - 3, position.z + z - 3, BlockIDs::LEAVES);
			}
		}
	}
}

// Trees
//for(int i = 0; i < treePositions.size(); i++) {
//	const int treeHeight = (rand() % 6) + 4;
//
//	const int treeX = treePositions[i].x;
//	const int treeY = treePositions[i].y;
//	const int treeZ = treePositions[i].z;
//
//	// Wood
//	for(int height = 0; height < treeHeight; height++) {
//		chunk.SetBlock(treeX, treeY + height, treeZ, BlockIDs::WOOD);
//	}
//
//	// Leaves
//	for(int y = 0; y < 6; y++) {
//		for(int z = 0; z < 7; z++) {
//			for(int x = 0; x < 7; x++) {
//				if(TREE_STRUCTURE[y][z][x] == 1)
//					chunk.SetBlock(treeX + x - TREE_OFFSET.x, treeY + y + treeHeight - TREE_OFFSET.y, treeZ + z - TREE_OFFSET.z, BlockIDs::LEAVES);
//			}
//		}
//	}
//}