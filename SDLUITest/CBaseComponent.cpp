#include "pch.h"
#include "CBaseComponent.h"


CBaseComponent::CBaseComponent()
{
	this->_Position = glm::vec3(0.f, 0.f, 0.f);
	this->_Rotation = glm::vec3(0.f);
	this->_Scale = glm::vec3(1.f);
	this->CalculateMatrix();
}


CBaseComponent::~CBaseComponent()
{
	CLog::MyLog(0, "BaseComponentDestructor %s",this->GetName().c_str());
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
	this->SetPosition(glm::vec3(0.0f));
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

void CBaseComponent::SetType(int type)
{
	this->_Type = type;
}

int CBaseComponent::GetType()
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

	FV.x = this->_Position.x + (cos(glm::radians(this->_Rotation.x))*cos(glm::radians(this->_Rotation.y)));
	FV.y = this->_Position.y + sin(glm::radians(this->_Rotation.x));
	FV.z = this->_Position.z + (cos(glm::radians(this->_Rotation.x))*sin(glm::radians(this->_Rotation.y)));

	//CLog::MyLog(0, "x=" + std::to_string(FV.x) + " y=" + std::to_string(FV.y) + " z=" + std::to_string(FV.z));
	return FV;
}

glm::vec3 CBaseComponent::GetRightVector()
{
	glm::vec3 RV;
	RV.x = this->_Position.x + (cos(glm::radians(this->_Rotation.z))*cos(glm::radians(this->_Rotation.y+90.f)));
	RV.y = this->_Position.y + sin(glm::radians(this->_Rotation.z));
	RV.z = this->_Position.z + (cos(glm::radians(this->_Rotation.z))*sin(glm::radians(this->_Rotation.y+90.f)));
	//CLog::MyLog(0, "RVector x=%f y=%f z=%f",RV.x,RV.y,RV.z);
	return RV;
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

void CBaseComponent::Tick(uint32_t delta)
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

void CBaseComponent::Draw()
{
}
