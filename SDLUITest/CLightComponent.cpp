#include "pch.h"
#include "CLightComponent.h"
#include "CObject3D.h"

CLightComponent::CLightComponent(CObject3D* ref) : CBaseComponent(ref)
{
	this->_Type = Object3DComponent::LIGHT_COMPONENT;
	this->Light.Ambient = glm::vec3(1.0f);
	this->Light.Color = glm::vec3(0.0f, 1.0f, 1.0f);
	this->Light.Diffuse = glm::vec3(1.0f);
	this->Light.Specular = glm::vec3(1.0f);
	this->Light.Constant = 1.0f;
	this->Light.Linear = 0.f;
	this->Light.Quadratic = 0.f;
	this->Light.CutoutDist = 9999.f;
	this->Light.LightType = LightType::Directional;
	this->_Scale = glm::vec3(0.5f);
}

CLightComponent::CLightComponent(const CLightComponent& light, CObject3D* ref) :CBaseComponent(light, ref)
{
	this->_Type = Object3DComponent::LIGHT_COMPONENT;
	this->Light.Ambient = light.Light.Ambient;
	this->Light.Diffuse = light.Light.Diffuse;
	this->Light.Color = light.Light.Color;
	this->Light.Constant = light.Light.Constant;
	this->Light.CutoutDist = light.Light.CutoutDist;
	this->Light.LightType = light.Light.LightType;
	this->Light.Linear = light.Light.Linear;
	this->Light.Quadratic = light.Light.Quadratic;
	this->Light.Specular = light.Light.Specular;
	this->Light.Position = glm::vec3(0.f);
	this->Light.Rotation = glm::vec3(0.f);
	this->AttachParrentObject(this->PossesingObject->GetRootComponent());
	this->CalculateMatrix();
}

CLightComponent::~CLightComponent()
{
}

void CLightComponent::Tick(double delta)
{
	CBaseComponent::Tick(delta);

	this->Light.Position = this->_Position;
	this->Light.Rotation = this->_Rotation;
}

void CLightComponent::SetLightColor(glm::vec3 Color)
{
	this->Light.Color = Color;
}

void CLightComponent::SetLightBaseData(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular)
{
	this->Light.Ambient = Ambient;
	this->Light.Diffuse = Diffuse;
	this->Light.Specular = Specular;
}

void CLightComponent::SetLightPointLightData(float constant, float linear, float quadriatic)
{
	this->Light.Constant = constant;
	this->Light.Linear = linear;
	this->Light.Quadratic = quadriatic;
}

void CLightComponent::SetLightDistanceCutOff(float CutOffDistance)
{
	this->Light.CutoutDist = CutOffDistance;
}

void CLightComponent::SetLightType(LightType type)
{
	this->Light.LightType = type;
}

Light CLightComponent::GetLightStruct()
{
	return this->Light;
}