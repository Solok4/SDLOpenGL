#pragma once
#include "CBaseComponent.h"

enum LightType
{
	Directional = 0,
	Point,
	Spotlight
};

class CLightComponent :
	public CBaseComponent
{
public:
	CLightComponent(CObject3D* ref);
	CLightComponent(const CLightComponent& light, CObject3D* ref);
	~CLightComponent();
	//Function called every frame.

	void SetLightColor(glm::vec3 Color);
	void SetLightBaseData(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular);
	void SetLightPointLightData(float constant, float linear, float quadriatic);
	void SetLightDistanceCutOff(float CutOffDistance);
	void SetLightType(LightType type);

	glm::vec3 GetAmbient();
	glm::vec3 GetDiffuse();
	glm::vec3 GetSpecular();
	glm::vec3 GetColor();
	LightType GetLightType();
	float GetConstant();
	float GetLinear();
	float GetQuadratic();
	float GetCutoutDist();

private: 
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	glm::vec3 _color;

	LightType _lightType;

	float _constant = 0;
	float _linear = 0;
	float _quadratic = 0;

	float _cutoutDist = 0;
};
