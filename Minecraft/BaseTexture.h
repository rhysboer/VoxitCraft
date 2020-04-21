#pragma once
#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

struct TextureData {
	unsigned char* data;
	int width;
	int height;
	int format;

	int GetFormat() const {
		return (format == 3) ? GL_RGB : GL_RGBA;
	}
};

class BaseTexture {
public:
	~BaseTexture();

	virtual void BindTexture(const unsigned int& index = 0) const;
	unsigned int GetTexture() const;

protected:
	BaseTexture();

	TextureData* LoadFile(const char* path, const bool& flipY = true) const;
	virtual void GenerateTexture(const TextureData& data);
	void DestroyTextureData(TextureData* data);
	
	unsigned int texture;
};

