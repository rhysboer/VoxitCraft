#pragma once
#include "Object3D.h"
#include "Cubemap.h"

#include "BaseCamera.h"

class SkyBox {
public:
	
	SkyBox(const char* cubemapPath);
	~SkyBox();

	void Render(BaseCamera& camera);
	Shader* GetShader();

private:

	Cubemap* texture;
	Object3D* cube;
};

