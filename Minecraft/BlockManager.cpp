#include "BlockManager.h"

std::vector<BlockData> BlockManager::blocks = std::vector<BlockData>();
TileMap* BlockManager::blockTexture = nullptr;
TileMap* BlockManager::waterTexture = nullptr;


// Vertex Block Data
std::vector<float> BlockManager::vertexDataBlock = std::vector<float>(
	{	/* VERTEX POS		NORMALS */
		// Front
		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,-1.0f,
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f,-1.0f,
		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,-1.0f,
		1.0f, 1.0f, 0.0f,	0.0f, 0.0f,-1.0f,
								
		// Right
		1.0f, 0.0f, 1.0f,	1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,	1.0f, 0.0f, 0.0f,

		// Back
		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,			
		1.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,			
		1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f,			
		0.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	

		// Left
		0.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,

		// Up
		1.0f, 1.0f, 1.0f,	0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 0.0f,

		// Down
		0.0f, 0.0f, 1.0f,	0.0f,-1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,	0.0f,-1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,	0.0f,-1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,	0.0f,-1.0f, 0.0f
	}
);

// Vertex Block Water
std::vector<float> BlockManager::vertexDataWater = std::vector<float>(
	{	/* VERTEX POS		NORMALS */
		// Front
		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,-1.0f,
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f,-1.0f,
		0.0f, 0.9f, 0.0f,	0.0f, 0.0f,-1.0f,
		1.0f, 0.9f, 0.0f,	0.0f, 0.0f,-1.0f,

		// Right
		1.0f, 0.0f, 1.0f,	1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		1.0f, 0.9f, 0.0f,	1.0f, 0.0f, 0.0f,
		1.0f, 0.9f, 1.0f,	1.0f, 0.0f, 0.0f,

		// Back
		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		1.0f, 0.9f, 1.0f,	0.0f, 0.0f, 1.0f,
		0.0f, 0.9f, 1.0f,	0.0f, 0.0f, 1.0f,

		// Left
		0.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
		0.0f, 0.9f, 1.0f,  -1.0f, 0.0f, 0.0f,
		0.0f, 0.9f, 0.0f,  -1.0f, 0.0f, 0.0f,

		// Up
		1.0f, 0.9f, 1.0f,	0.0f, 1.0f, 0.0f,
		1.0f, 0.9f, 0.0f,	0.0f, 1.0f, 0.0f,
		0.0f, 0.9f, 0.0f,	0.0f, 1.0f, 0.0f,
		0.0f, 0.9f, 1.0f,	0.0f, 1.0f, 0.0f,

		// Down
		0.0f, 0.0f, 1.0f,	0.0f,-1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,	0.0f,-1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,	0.0f,-1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,	0.0f,-1.0f, 0.0f
	}
);

std::vector<float> BlockManager::vertexDataX = std::vector<float>(
	{
		0.8535f, 0.0f, 0.8535f,	0.0f, 1.0f, 0.0f,
		0.1465f, 0.0f, 0.1465f,	0.0f, 1.0f, 0.0f,
		0.1465f, 1.0f, 0.1465f,	0.0f, 1.0f, 0.0f,
		0.8535f, 1.0f, 0.8535f,	0.0f, 1.0f, 0.0f,
										    
		0.1465f, 0.0f, 0.1465f,	0.0f, 1.0f, 0.0f,
		0.8535f, 0.0f, 0.8535f,	0.0f, 1.0f, 0.0f,
		0.8535f, 1.0f, 0.8535f,	0.0f, 1.0f, 0.0f,
		0.1465f, 1.0f, 0.1465f,	0.0f, 1.0f, 0.0f,
				 	  					    
		0.1465f, 0.0f, 0.8535f,	0.0f, 1.0f, 0.0f,
		0.8535f, 0.0f, 0.1465f,	0.0f, 1.0f, 0.0f,
		0.8535f, 1.0f, 0.1465f,	0.0f, 1.0f, 0.0f,
		0.1465f, 1.0f, 0.8535f,	0.0f, 1.0f, 0.0f,
										   
		0.8535f, 0.0f, 0.1465f,	0.0f, 1.0f, 0.0f,
		0.1465f, 0.0f, 0.8535f,	0.0f, 1.0f, 0.0f,
		0.1465f, 1.0f, 0.8535f,	0.0f, 1.0f, 0.0f,
		0.8535f, 1.0f, 0.1465f,	0.0f, 1.0f, 0.0f,
	}
);

// Call once on startup
void BlockManager::Init() {
	// Load block textures
	blockTexture = new TileMap("./data/textures/tileset.png", 8, 8);
	waterTexture = new TileMap("./data/textures/water.png", 16, 1);

	// Load Block Data
	LoadBlockDatabase();
}

// Call once on shut down
void BlockManager::Destroy() {
	delete blockTexture;
	delete waterTexture;

	blockTexture = nullptr;
	waterTexture = nullptr;
}

const BlockData const * BlockManager::GetBlockData(const BlockIDs& id) {
	return &blocks[(int)id];
}

TileMap const* BlockManager::GetTileMap(const SpriteSheet type) {
	return (type == SpriteSheet::BLOCK) ? blockTexture : waterTexture;
}

void BlockManager::GetTextureCoords(const BlockIDs& block, const unsigned int& face, std::array<glm::vec2, 4>& coords) {
	BlockData const* data = GetBlockData(block);

	if(data->spriteSheet == SpriteSheet::BLOCK) {
		blockTexture->GetSpriteCoordinates(data->texture[face], coords);
	} else {
		waterTexture->GetSpriteCoordinates(data->texture[face], coords);
	}
}

int BlockManager::TotalBlocks() {
	return blocks.size();
}

int BlockManager::GetMeshFaceCount(const MeshType& type) {
	switch(type) {
		case MeshType::LIQUID:
		case MeshType::BLOCK: return 6;
		case MeshType::X: return 4;
		default: return 6;
	}
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
	blocks.push_back(BlockData(BlockIDs::BUSH, "Bush", true, false, false, TextureIndex(10, 10, 10, 10, 0, 0), MeshType::X));
	blocks.push_back(BlockData(BlockIDs::WATER, "Water", true, false, true, TextureIndex(0, 0, 0, 0, 0, 0), MeshType::LIQUID, SpriteSheet::WATER));	
}