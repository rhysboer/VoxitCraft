#pragma once
#include <string>
#include "BaseTexture.h"

class Cubemap : public BaseTexture {
public:
	Cubemap(const char* path);

	void BindTexture(const unsigned int& index = 0) const override;

private:

	void LoadCubemap(const char* path);
};

