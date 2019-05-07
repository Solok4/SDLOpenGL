#pragma once
#include "CBaseComponent.h"

enum LightType
{
	Directional = 0,
	Point,
	Spotlight
};

struct Light
{
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Color;

	int LightType;

	float Constant;
	float Linear;
	float Quadratic;

	float CutoutDist;
};


class CLightComponent :
	public CBaseComponent
{
public:
	CLightComponent();
	~CLightComponent();

	void Tick(uint32_t delta) override;
	void SetLightColor(glm::vec3 Color);
	void SetLightBaseData(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular);
	void SetLightPointLightData(float constant, float linear, float quadriatic);
	void SetLightDistanceCutOff(float CutOffDistance);
	void SetLightType(LightType type);
	Light GetLightStruct();

	Light Light;
};

