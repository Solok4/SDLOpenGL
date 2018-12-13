#include "pch.h"
#include "CBaseComponent.h"


CBaseComponent::CBaseComponent()
{
	std::shared_ptr<CBaseComponent> temp(this);
	this->_Pointer = temp;

	this->_Position = glm::vec3(0.f, 0.f, -20.f);
	this->_Rotation = glm::vec3(0.f);
	this->_Scale = glm::vec3(1.f);
	this->CalculateMatrix();
}


CBaseComponent::~CBaseComponent()
{
}

std::shared_ptr<CBaseComponent> CBaseComponent::GetPointer()
{
	return this->_Pointer;
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
	if (this->_ParrentObject == nullptr)
	{
		glm::mat4 Translation = glm::translate(glm::mat4(), glm::vec3(this->_Position.x, this->_Position.y, this->_Position.z));
		glm::mat4 Scaling = glm::scale(glm::vec3(this->_Scale));
		glm::mat4 RotationX = glm::rotate(glm::radians(this->_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 RotationY = glm::rotate(glm::radians(this->_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 RotationZ = glm::rotate(glm::radians(this->_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 Rotation = RotationX * RotationY *RotationZ;
		this->ModelMatrix = Translation * Rotation* Scaling;
	}
	else
	{
		glm::mat4 Translation = glm::translate(glm::mat4(), glm::vec3(this->_ParrentObject->_Position + this->_Position));
		glm::mat4 Scaling = glm::scale(glm::vec3(this->_Scale));
		glm::mat4 RotationX = glm::rotate(glm::radians(this->_ParrentObject->_Rotation.x + this->_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 RotationY = glm::rotate(glm::radians(this->_ParrentObject->_Rotation.y + this->_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 RotationZ = glm::rotate(glm::radians(this->_ParrentObject->_Rotation.z + this->_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 Rotation = RotationX * RotationY *RotationZ;
		this->ModelMatrix = Translation * Rotation* Scaling;
	}
	
}

void CBaseComponent::SetPosition(glm::vec3 pos)
{
	this->_Position = pos;
}

void CBaseComponent::SetRotation(glm::vec3 rot)
{
	this->_Rotation = rot;
}

void CBaseComponent::SetScale(glm::vec3 scale)
{
	this->_Scale = scale;
}

glm::mat4 CBaseComponent::GetModelMatrix()
{
	return this->ModelMatrix;
}

void CBaseComponent::Draw()
{
}
