#pragma once
#include "Object3D.h"
#include "Cubemap.h"

class SkyBox {
public:
	
	SkyBox(const char* cubemapPath);
	~SkyBox();

	void Render(Camera& camera);

private:

	Cubemap* texture;
	Object3D* cube;
};

