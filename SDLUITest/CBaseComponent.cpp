#include "pch.h"
#include "CBaseComponent.h"


CBaseComponent::CBaseComponent()
{
	this->_Type = Object3DComponent::BASE_COMPONENT;
	this->_Position = glm::vec3(0.f, 0.f, 0.f);
	this->_Rotation = glm::vec3(0.f);
	this->_Scale = glm::vec3(1.f);
	this->CalculateMatrix();
}

CBaseComponent::CBaseComponent(const CBaseComponent& comp)
{
	this->_Name = comp._Name;
	this->_Type = comp._Type;
	this->_Position = comp._Position;
	this->_Rotation = comp._Rotation;
	this->_Scale = comp._Scale;
	this->_RotOffset = comp._RotOffset;
	this->_PosOffset = comp._PosOffset;
}

CBaseComponent::CBaseComponent(const CBaseComponent& comp, char* name)
{
	this->_Name = name;
	this->_Type = comp._Type;
	this->_Position = comp._Position;
	this->_Rotation = comp._Rotation;
	this->_Scale = comp._Scale;
	this->_RotOffset = comp._RotOffset;
	this->_PosOffset = comp._PosOffset;
}

CBaseComponent::CBaseComponent(const std::shared_ptr<CBaseComponent>& obj)
{
	this->_Name = obj->_Name;
	this->_Type = obj->_Type;
	this->_Position = obj->_Position;
	this->_Rotation = obj->_Rotation;
	this->_Scale = obj->_Scale;
	this->_RotOffset = obj->_RotOffset;
	this->_PosOffset = obj->_PosOffset;
}

CBaseComponent::CBaseComponent(const std::shared_ptr<CBaseComponent>& obj, char* name)
{
	this->_Name = name;
	this->_Type = obj->_Type;
	this->_Position = obj->_Position;
	this->_Rotation = obj->_Rotation;
	this->_Scale = obj->_Scale;
	this->_RotOffset = obj->_RotOffset;
	this->_PosOffset = obj->_PosOffset;
}


CBaseComponent::~CBaseComponent()
{
	CLog::MyLog(LogType::Log, "BaseComponentDestructor %s",this->GetName().c_str());
}

void CBaseComponent::SetName(std::string name)
{
	this->_Name = name;
}

std::string CBaseComponent::GetName()
{
	return this->_Name;
}

void CBaseComponent::AttachParrentObject(std::shared_ptr<CBaseComponent> Parrent)
{
	this->_ParrentObject = Parrent;
	//this->SetPosition(glm::vec3(0.0f));
	this->CalculateMatrix();
}

void CBaseComponent::SetPossesingObject(std::shared_ptr<CObject3D> obj)
{
	if (obj != nullptr)
	{
		this->PossesingObject = obj;
	}
}

std::shared_ptr<CObject3D> CBaseComponent::GetPossesingObject()
{
	return this->PossesingObject;
}

void CBaseComponent::SetType(Object3DComponent type)
{
	this->_Type = type;
}

Object3DComponent CBaseComponent::GetType()
{
	return this->_Type;
}

void CBaseComponent::CalculateMatrix()
{

	glm::mat4 Translation = glm::translate(glm::mat4(), glm::vec3(this->_Position.x, this->_Position.y, this->_Position.z));
	glm::mat4 Scaling = glm::scale(glm::vec3(this->_Scale));
	glm::mat4 RotationX = glm::rotate(glm::radians(this->_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 RotationY = glm::rotate(glm::radians(this->_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 RotationZ = glm::rotate(glm::radians(this->_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 Rotation = RotationX * RotationY *RotationZ;
	this->ModelMatrix = Translation * Rotation* Scaling;
}

glm::vec3 CBaseComponent::GetForwardVector()
{
	glm::vec3 FV;

	FV.x = (cos(glm::radians(this->_Rotation.x))*cos(glm::radians(this->_Rotation.y)));
	FV.y = sin(glm::radians(this->_Rotation.x));
	FV.z = (cos(glm::radians(this->_Rotation.x))*sin(glm::radians(this->_Rotation.y)));
	return FV;
}

glm::vec3 CBaseComponent::GetRightVector()
{
	glm::vec3 RV;
	RV.x = (cos(glm::radians(this->_Rotation.z))*cos(glm::radians(this->_Rotation.y+90.f)));
	RV.y = sin(glm::radians(this->_Rotation.z));
	RV.z = (cos(glm::radians(this->_Rotation.z))*sin(glm::radians(this->_Rotation.y+90.f)));
	return RV;
}

glm::vec3 CBaseComponent::GetUpVector()
{
	glm::vec3 UV = glm::vec3(0.0f,1.0f,0.0f);
	return UV;
}

void CBaseComponent::SetPosition(glm::vec3 pos)
{
	if (this->_ParrentObject == nullptr)
	{
		this->_Position = pos;
	}
	else
	{
		this->_PosOffset = pos;
		this->_Position = this->_ParrentObject->_Position + this->_PosOffset;
	}
	this->CalculateMatrix();
}

glm::vec3 CBaseComponent::GetPosition()
{
	return this->_Position;
}

void CBaseComponent::SetRotation(glm::vec3 rot)
{
	if (this->_ParrentObject == nullptr)
	{
		this->_Rotation = rot;
	}
	else
	{
		this->_RotOffset = rot;
		this->_Rotation = this->_ParrentObject->_Rotation + this->_RotOffset;
	}
	this->CalculateMatrix();
}

glm::vec3 CBaseComponent::GetRotation()
{
	return this->_Rotation;
}

void CBaseComponent::SetScale(glm::vec3 scale)
{
	this->_Scale = scale;
	this->CalculateMatrix();
}


void CBaseComponent::Tick(double delta)
{
	if (this->_ParrentObject != nullptr)
	{
		this->_Position = this->_ParrentObject->_Position + this->_PosOffset;
		this->_Rotation = this->_ParrentObject->_Rotation + this->_RotOffset;
	}
}

glm::vec3 CBaseComponent::GetScale()
{
	return this->_Scale;
}

glm::mat4 CBaseComponent::GetModelMatrix()
{
	return this->ModelMatrix;
}

