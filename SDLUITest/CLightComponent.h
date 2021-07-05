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

	float Constant = 0;
	float Linear = 0;
	float Quadratic = 0;

	float CutoutDist = 0;
};

class CLightComponent :
	public CBaseComponent
{
public:
	CLightComponent(CObject3D* ref);
	CLightComponent(const CLightComponent& light, CObject3D* ref);
	~CLightComponent();
	//Function called every frame.
	void Tick(double delta) override;
	//Sets light colour.
	void SetLightColor(glm::vec3 Color);
	//Sets Ambient, diffuse and specular elements of light.
	void SetLightBaseData(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular);
	//Sets point light data.
	void SetLightPointLightData(float constant, float linear, float quadriatic);
	//Sets point light data.
	void SetLightDistanceCutOff(float CutOffDistance);
	//Set the light type.
	void SetLightType(LightType type);
	//Returns Light data struct.
	Light GetLightStruct();
private:
	Light Light;
};
