#include "CLightComponent.h"
#include "CObject3D.h"

CLightComponent::CLightComponent(CObject3D* ref) : CBaseComponent(ref)
{
	this->_Type = Object3DComponent::LIGHT_COMPONENT;
	this->_ambient = glm::vec3(1.0f);
	this->_color = glm::vec3(0.0f, 1.0f, 1.0f);
	this->_diffuse = glm::vec3(1.0f);
	this->_specular = glm::vec3(1.0f);
	this->_constant = 1.0f;
	this->_linear = 0.f;
	this->_quadratic = 0.f;
	this->_cutoutDist = 9999.f;
	this->_lightType = LightType::Directional;
	this->_Scale = glm::vec3(0.5f);
}

CLightComponent::CLightComponent(const CLightComponent& light, CObject3D* ref) :CBaseComponent(light, ref)
{
	this->_Type = Object3DComponent::LIGHT_COMPONENT;
	this->_ambient = light._ambient;
	this->_diffuse = light._diffuse;
	this->_color = light._color;
	this->_constant = light._constant;
	this->_cutoutDist = light._cutoutDist;
	this->_lightType = light._lightType;
	this->_linear = light._linear;
	this->_quadratic = light._quadratic;
	this->_specular = light._specular;
	this->AttachParrentObject(this->PossesingObject->GetRootComponent());
	this->CalculateMatrix();
}

CLightComponent::~CLightComponent()
{
}
void CLightComponent::SetLightColor(glm::vec3 Color)
{
	this->_color = Color;
}

void CLightComponent::SetLightBaseData(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular)
{
	this->_ambient = Ambient;
	this->_diffuse = Diffuse;
	this->_specular = Specular;
}

void CLightComponent::SetLightPointLightData(float constant, float linear, float quadriatic)
{
	this->_constant = constant;
	this->_linear = linear;
	this->_quadratic = quadriatic;
}

void CLightComponent::SetLightDistanceCutOff(float CutOffDistance)
{
	this->_cutoutDist = CutOffDistance;
}

void CLightComponent::SetLightType(LightType type)
{
	this->_lightType = type;
}

glm::vec3 CLightComponent::GetAmbient()
{
	return this->_ambient;
}

glm::vec3 CLightComponent::GetDiffuse()
{
	return this->_diffuse;
}
glm::vec3 CLightComponent::GetSpecular()
{
	return this->_specular;
}
glm::vec3 CLightComponent::GetColor()
{
	return this->_color;
}
LightType CLightComponent::GetLightType()
{
	return this->_lightType;
}
float CLightComponent::GetConstant()
{
	return this->_constant;
}
float CLightComponent::GetLinear()
{
	return this->_linear;
}
float CLightComponent::GetQuadratic()
{
	return this->_quadratic;
}
float CLightComponent::GetCutoutDist()
{
	return this->_cutoutDist;
}
