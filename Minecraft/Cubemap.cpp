#include "Cubemap.h"


Cubemap::Cubemap(const char* path) {
	LoadCubemap(path);
}

Cubemap::~Cubemap() {
	glDeleteTextures(1, &texture);
}

void Cubemap::BindTexture(const unsigned int& index) const {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

void Cubemap::LoadCubemap(const char* path) {
	std::string file[6] = { "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" };
	std::string directory = path;

	TextureData* textureData;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	for(int i = 0; i < 6; i++) {
		textureData = LoadFile((directory + file[i]).c_str(), false);

		// Failed
		if(textureData == nullptr) {
			printf("Cubemap: ERROR - Couldn't find texture at (%s)\n", (directory + file[i]).c_str());
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, textureData->width, textureData->height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData->data);

		DestroyTextureData(textureData);
		delete textureData;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}