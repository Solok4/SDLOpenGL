#pragma once
#include "CBaseComponent.h"

enum LightType
{
	DirectionalLight = 0,
	PointLight,
	SpotLight,
};


class CLightComponent :
	public CBaseComponent
{
public:
	CLightComponent();
	~CLightComponent();



	//For all types of lighting
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	//For pointlight. It also requires position and rotation.
	float Constant;
	float Linear;
	float Quadratic;

	//For spotlight
	float CutOffDist;

	LightType LightType;
};

