#pragma once
#include <vector>
#include <array>
#include "glm/glm.hpp"
#include "TileMap.h"

typedef unsigned int uint;

// IDs for every single block
enum class BlockIDs {
	AIR = 0,
	DIRT,
	GRASS,
	STONE,
	WOOD,
	LEAVES,
	GLASS,
	CACTUS,
	SAND,
	SNOW_DIRT,
	WOOD_PLANKS,
	MAGMA,
	BUSH,
	WATER,

	// Block type error
	_TOTAL
};

enum class SpriteSheet {
	BLOCK,
	WATER
};

enum class MeshType {
	BLOCK, LIQUID, X,
};

struct TextureIndex {
	enum class Face { FRONT = 0, RIGHT, BACK, LEFT, UP, DOWN };

	TextureIndex(const uint& front = 0, const uint& right = 0, const uint& back = 0, const uint& left = 0, const uint& up = 0, const uint& down = 0) {
		index = std::array<uint, 6>();
		index[(int)Face::FRONT] = front;
		index[(int)Face::RIGHT] = right;
		index[(int)Face::BACK] = back;
		index[(int)Face::LEFT] = left;
		index[(int)Face::UP] = up;
		index[(int)Face::DOWN] = down;
	}

	void operator = (const TextureIndex& other) {
		for(int i = 0; i < 6; i++)
			this->index[i] = other.index[i];
	}

	uint operator [](uint i) const {
		if(i > index.size())
			return this->index[5];
		return this->index[i];
	}

	std::array<uint, 6> index;
};

// Data about a block type
struct BlockData {
	TextureIndex texture;
	SpriteSheet spriteSheet;
	BlockIDs id;
	const char* name;
	bool walkThrough;
	bool isTransparent;
	bool useAmbient;
	int lightLevel;
	MeshType meshType;

	BlockData(BlockIDs _id, const char* _name, bool transparent, bool walkThrough, bool useAmbient, TextureIndex texture, int lightLevel = 0, MeshType mesh = MeshType::BLOCK, SpriteSheet spriteSheet = SpriteSheet::BLOCK)
		: id(_id), name(_name), isTransparent(transparent), walkThrough(walkThrough), useAmbient(useAmbient), lightLevel(lightLevel), meshType(mesh), spriteSheet(spriteSheet) {
		this->texture = texture;
	}
};

class BlockManager {
public:

	static void Init();
	static void Destroy();

	static const BlockData const* GetBlockData(const BlockIDs& id);
	static const TileMap const* GetTileMap(const SpriteSheet type = SpriteSheet::BLOCK);
	static void GetTextureCoords(const BlockIDs& block, const unsigned int& face, std::array<glm::vec2, 4>& coords);

	static int TotalBlocks();

	inline static void GetMeshFace(const MeshType& type, const TextureIndex::Face& face, const unsigned int& corner, const glm::vec3& pos, std::vector<float>& vertices) {
		int spot = ((int)face * 24) + (corner * 6);
		if(type == MeshType::BLOCK) {
			vertices.emplace_back(vertexDataBlock[spot + 0] + pos.x);	// Vertex X
			vertices.emplace_back(vertexDataBlock[spot + 1] + pos.y);	// Vertex Y
			vertices.emplace_back(vertexDataBlock[spot + 2] + pos.z);	// Vertex Z
			vertices.emplace_back(vertexDataBlock[spot + 3]);			// Normal X
			vertices.emplace_back(vertexDataBlock[spot + 4]);			// Normal Y
			vertices.emplace_back(vertexDataBlock[spot + 5]);			// Normal Z
		} else if(type == MeshType::LIQUID) {
			vertices.emplace_back(vertexDataWater[spot + 0] + pos.x);	// Vertex X
			vertices.emplace_back(vertexDataWater[spot + 1] + pos.y);	// Vertex Y
			vertices.emplace_back(vertexDataWater[spot + 2] + pos.z);	// Vertex Z
			vertices.emplace_back(vertexDataWater[spot + 3]);			// Normal X
			vertices.emplace_back(vertexDataWater[spot + 4]);			// Normal Y
			vertices.emplace_back(vertexDataWater[spot + 5]);			// Normal Z
		} else {
			vertices.emplace_back(vertexDataX[spot + 0] + pos.x);	// Vertex X
			vertices.emplace_back(vertexDataX[spot + 1] + pos.y);	// Vertex Y
			vertices.emplace_back(vertexDataX[spot + 2] + pos.z);	// Vertex Z
			vertices.emplace_back(vertexDataX[spot + 3]);			// Normal X
			vertices.emplace_back(vertexDataX[spot + 4]);			// Normal Y
			vertices.emplace_back(vertexDataX[spot + 5]);			// Normal Z
		}
	}
	static int GetMeshFaceCount(const MeshType& type);

private:
	~BlockManager() {};
	BlockManager() {};
	
	static void LoadBlockDatabase();

	static TileMap* blockTexture;
	static TileMap* waterTexture;

	static std::vector<BlockData> blocks;

	// Mesh Data
	static std::vector<float> vertexDataBlock;
	static std::vector<float> vertexDataWater;
	static std::vector<float> vertexDataX;
};

