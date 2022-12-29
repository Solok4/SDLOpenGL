#pragma once

#include "../shared/Shared.h"

enum SkyboxType
{
	CubeType = 0,
};

class CSkybox {
public:

private:
	GLuint SkyboxVAO;
	GLuint SkyboxVBOs[2];
	SkyboxType Skyboxtype;
};