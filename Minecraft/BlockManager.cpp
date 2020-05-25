#include "BlockManager.h"

std::vector<BlockData> BlockManager::blocks = std::vector<BlockData>();

BlockManager::BlockManager() {
}

BlockManager::~BlockManager() {
}

const BlockData const * BlockManager::GetBlockData(const BlockIDs& id) {
	return &blocks[(int)id];
}

int BlockManager::TotalBlocks() {
	return blocks.size();
}

void BlockManager::LoadBlockDatabase() {
	blocks.reserve((int)BlockIDs::_TOTAL);
	blocks.push_back(BlockData(BlockIDs::AIR, "Air", true, false, false, TextureIndex()));
	blocks.push_back(BlockData(BlockIDs::DIRT, "Dirt", false, true, true, TextureIndex(0, 0, 0, 0, 0, 0)));
	blocks.push_back(BlockData(BlockIDs::GRASS, "Grass", false, true, true, TextureIndex(1, 1, 1, 1, 2, 0)));
	blocks.push_back(BlockData(BlockIDs::STONE, "Stone", false, true, true, TextureIndex(7, 7, 7, 7, 7, 7)));
	blocks.push_back(BlockData(BlockIDs::WOOD, "Oak", false, true, true, TextureIndex(3, 3, 3, 3, 4, 4)));
	blocks.push_back(BlockData(BlockIDs::LEAVES, "Leaves", false, true, true, TextureIndex(6, 6, 6, 6, 6, 6)));
	blocks.push_back(BlockData(BlockIDs::GLASS, "Glass", true, true, false, TextureIndex(11, 11, 11, 11, 11, 11)));
	blocks.push_back(BlockData(BlockIDs::CACTUS, "Cactus", false, true, true, TextureIndex(12, 12, 12, 12, 13, 14)));
	blocks.push_back(BlockData(BlockIDs::SAND, "Sand", false, true, true, TextureIndex(8, 8, 8, 8, 8, 8)));
	blocks.push_back(BlockData(BlockIDs::SNOW_DIRT, "Snowy Dirt", false, true, true, TextureIndex(15, 15, 15, 15, 16, 0)));
	blocks.push_back(BlockData(BlockIDs::WOOD_PLANKS, "Wooden Planks", false, true, true, TextureIndex(17, 17, 17, 17, 17, 17)));
	blocks.push_back(BlockData(BlockIDs::MAGMA, "Magma", false, true, true, TextureIndex(18, 18, 18, 18, 18, 18)));
	blocks.push_back(BlockData(BlockIDs::WATER, "Water", true, false, true, TextureIndex(0, 0, 0, 0, 0, 0)));
	
}
