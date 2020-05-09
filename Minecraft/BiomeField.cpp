#include "BiomeField.h"
#include "World.h"

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

	std::vector<glm::vec3> blockPos = std::vector<glm::vec3>();
	std::vector<BlockIDs> blockIds = std::vector<BlockIDs>();

	for(int i = 0; i < height; i++) {
		//chunk.SetBlock(position.x, position.y + i, position.z, BlockIDs::WOOD);
		blockPos.emplace_back(position.x, position.y + i, position.z);
		blockIds.emplace_back(BlockIDs::WOOD);
	}

	for(int y = 0; y < 6; y++) {
		for(int z = 0; z < 7; z++) {
			for(int x = 0; x < 7; x++) {
				if(TREE_STRUCTURE[y][z][x] == 1) {
					blockPos.emplace_back(position.x + x - 3, position.y + y + height - 3, position.z + z - 3); //chunk.SetBlock(position.x + x - 3, position.y + y + height - 3, position.z + z - 3, BlockIDs::LEAVES);
					blockIds.emplace_back(BlockIDs::LEAVES);
				}
			}
		}
	}

	World::GetChunkManager().SetBlocks(blockPos, blockIds);
}


/// TEST
void BiomeField::AddStructure(const glm::vec3& position, std::array<BlockIDs, Chunk::CHUNK_MASS>& chunkBlocks, std::vector<glm::vec3>& outPos, std::vector<BlockIDs>& outBlocks) {
	const int height = (rand() % 4) + 6;

	glm::vec3 pos = glm::vec3(0);
	glm::vec2 chunkPos = glm::vec2(glm::floor(position.x / 16.0f) * 16.0f, glm::floor(position.z / 16.0f) * 16.0f);

	// Should always be inside the chunk
	for(int i = 0; i < height; i++) {
		chunkBlocks[((position.y + i) * Chunk::CHUNK_SLICE) + ((position.z - chunkPos.y) * Chunk::CHUNK_SIZE) + (position.x - chunkPos.x)] = BlockIDs::WOOD;
		//outPos.emplace_back(position.x, position.y + i, position.z);
		//outBlocks.emplace_back(BlockIDs::WOOD);
	}


	for(int y = 0; y < 6; y++) {
		for(int z = 0; z < 7; z++) {
			for(int x = 0; x < 7; x++) {
				if(TREE_STRUCTURE[y][z][x] == 1) {
					pos = glm::vec3(position.x + x - 3, position.y + y + height - 3, position.z + z - 3);
	
					if(pos.x - chunkPos.x < 0 || pos.x - chunkPos.x >= Chunk::CHUNK_SIZE || pos.z - chunkPos.y < 0 || pos.z - chunkPos.y >= Chunk::CHUNK_SIZE) {
						outPos.emplace_back(position.x + x - 3, position.y + y + height - 3, position.z + z - 3);
						outBlocks.emplace_back(BlockIDs::LEAVES);
					} else {
						chunkBlocks[(pos.y * Chunk::CHUNK_SLICE) + ((pos.z - chunkPos.y) * Chunk::CHUNK_SIZE) + (pos.x - chunkPos.x)] = BlockIDs::LEAVES;
					}
				}
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