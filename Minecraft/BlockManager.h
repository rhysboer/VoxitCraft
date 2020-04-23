#pragma once
#include <vector>
#include <array>
#include "glm/glm.hpp"

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
	WATER,

	// Block type error
	_TOTAL
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
	BlockIDs id;
	const char* name;
	bool isSolid;
	bool isTransparent;

	BlockData(BlockIDs _id, const char* _name, bool transparent, bool solid, TextureIndex texture) : id(_id), name(_name), isTransparent(transparent), isSolid(solid) {
		this->texture = texture;
	}
};

class BlockManager {
public:

	static void LoadBlockDatabase();

	static const BlockData& GetBlockData(const BlockIDs& id);

private:
	~BlockManager();
	BlockManager();

	static std::vector<BlockData> blocks;
};

