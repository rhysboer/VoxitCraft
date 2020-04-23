#pragma once
#include <cmath>
#include "EntityController.h"
#include "Input.h"
#include "Time.h"

class ECPlayer : public EntityController {
public:

	ECPlayer();
	~ECPlayer();

private:

	// Inherited via EntityController
	virtual void OnUpdate() override;
};

