#include "pch.h"
#include "CLightComponent.h"


CLightComponent::CLightComponent()
{
	this->Light.Ambient = glm::vec3(1.0f);
	this->Light.Color = glm::vec3(0.0f, 1.0f, 1.0f);
	this->Light.Diffuse = glm::vec3(1.0f);
	this->Light.Specular = glm::vec3(1.0f);
	this->Light.Constant = 1.0f;
	this->Light.Linear = 0.f;
	this->Light.Quadratic = 0.f;
	this->Light.CutoutDist = 9999.f;
	this->Light.LightType = LightType::Directional;
}

CLightComponent::CLightComponent(const CLightComponent& light) :CBaseComponent(light)
{
	this->Light = light.Light;
}

CLightComponent::CLightComponent(std::shared_ptr<CLightComponent>& obj):CBaseComponent(obj)
{
	this->Light = obj->Light;
}


CLightComponent::~CLightComponent()
{
}

void CLightComponent::Tick(uint32_t delta)
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
