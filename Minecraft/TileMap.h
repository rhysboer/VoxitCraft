#pragma once
#include <array>
#include <cmath>
#include "glm/vec2.hpp"
#include "Sprite.h"

class TileMap {
public:

	TileMap(const char* path, const unsigned int& rows, const unsigned int& columns);
	~TileMap();

	void BindTexture(const unsigned int& index) const;

	//void GetSpriteCoordinates(const BlockIDs& block, const TextureIndex::Face& face, std::array<glm::ivec2, 4>& coords) const;
	void GetSpriteCoordinates(const unsigned int& index, std::array<glm::vec2, 4>& coords) const;
private:

	// Total sprites per axis
	unsigned int rows; // X
	unsigned int columns; // Y

	// Size of each sprite
	float sizeX;
	float sizeY;

	Sprite* texture;
};

