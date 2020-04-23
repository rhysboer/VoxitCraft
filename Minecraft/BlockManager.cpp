#include "BlockManager.h"

std::vector<BlockData> BlockManager::blocks = std::vector<BlockData>();

BlockManager::BlockManager() {
}

BlockManager::~BlockManager() {
}

const BlockData& BlockManager::GetBlockData(const BlockIDs& id) {
	return blocks[(int)id];
}

void BlockManager::LoadBlockDatabase() {
	blocks.reserve((int)BlockIDs::_TOTAL);
	blocks.push_back(BlockData(BlockIDs::AIR, "Air", true, false, TextureIndex()));
	blocks.push_back(BlockData(BlockIDs::DIRT, "Dirt", false, true, TextureIndex(0, 0, 0, 0, 0, 0)));
	blocks.push_back(BlockData(BlockIDs::GRASS, "Grass", false, true, TextureIndex(1, 1, 1, 1, 2, 0)));
	blocks.push_back(BlockData(BlockIDs::STONE, "Stone", false, true, TextureIndex(7, 7, 7, 7, 7, 7)));
	blocks.push_back(BlockData(BlockIDs::WOOD, "Oak", false, true, TextureIndex(3, 3, 3, 3, 4, 4)));
	blocks.push_back(BlockData(BlockIDs::LEAVES, "Leaves", false, true, TextureIndex(6, 6, 6, 6, 6, 6)));
	blocks.push_back(BlockData(BlockIDs::GLASS, "Glass", true, true, TextureIndex(11, 11, 11, 11, 11, 11)));
	blocks.push_back(BlockData(BlockIDs::CACTUS, "Cactus", false, true, TextureIndex(12, 12, 12, 12, 13, 14)));
	blocks.push_back(BlockData(BlockIDs::SAND, "Sand", false, true, TextureIndex(8, 8, 8, 8, 8, 8)));
	blocks.push_back(BlockData(BlockIDs::SNOW_DIRT, "Snowy Dirt", false, true, TextureIndex(15, 15, 15, 15, 16, 0)));
	blocks.push_back(BlockData(BlockIDs::WATER, "Water", true, false, TextureIndex(0, 0, 0, 0, 0, 0)));
}
