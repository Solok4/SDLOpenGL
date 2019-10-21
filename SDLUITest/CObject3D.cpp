#include "pch.h"
#include "CObject3D.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

CObject3D::CObject3D()
{
	this->AddComponent(0, "root");
	this->BindRootComponent("root");
}

CObject3D::CObject3D(const CObject3D& obj):CObject3D()
{
	this->Name = obj.Name;
	if (obj._Components.size() != 0)
	{
		for (int i = 0; i < obj._Components.size(); i++)
		{
			if (obj._Components[i]->GetType() == CAMERA_COMPONENT)
			{
				auto temp = std::dynamic_pointer_cast<CCameraComponent>(obj._Components[i]);
				this->AddComponent(temp);
			}
			else if (obj._Components[i]->GetType() == STATIC_MESH_COMPONENT)
			{
				auto temp = std::dynamic_pointer_cast<CStaticMeshComponent>(obj._Components[i]);
				this->AddComponent(temp);
			}
			else if (obj._Components[i]->GetType() == MOVEMENT_COMPONENT)
			{
				auto temp = std::dynamic_pointer_cast<CMovementComponent>(obj._Components[i]);
				this->AddComponent(temp);
			}
			else if (obj._Components[i]->GetType() == LIGHT_COMPONENT)
			{
				auto temp = std::dynamic_pointer_cast<CLightComponent>(obj._Components[i]);
				this->AddComponent(temp);
			}
		}
	}
}

CObject3D::CObject3D(const std::shared_ptr<CObject3D>& obj) :CObject3D()
{
	this->Name = obj->Name;
	if (obj->_Components.size() != 0)
	{
		for (int i = 0; i < obj->_Components.size(); i++)
		{
			if (obj->_Components[i]->GetType() == CAMERA_COMPONENT)
			{
				auto temp = std::dynamic_pointer_cast<CCameraComponent>(obj->_Components[i]);
				this->AddComponent(temp);
			}
			else if (obj->_Components[i]->GetType() == STATIC_MESH_COMPONENT)
			{
				auto temp = std::dynamic_pointer_cast<CStaticMeshComponent>(obj->_Components[i]);
				this->AddComponent(temp);
			}
			else if (obj->_Components[i]->GetType() == MOVEMENT_COMPONENT)
			{
				auto temp = std::dynamic_pointer_cast<CMovementComponent>(obj->_Components[i]);
				this->AddComponent(temp);
			}
			else if (obj->_Components[i]->GetType() == LIGHT_COMPONENT)
			{
				auto temp = std::dynamic_pointer_cast<CLightComponent>(obj->_Components[i]);
				this->AddComponent(temp);
			}
		}
	}
}


CObject3D::~CObject3D()
{
	CLog::MyLog(LogType::Log, "3DObjectDestructor %s",this->GetName().c_str());
}

void CObject3D::Prepare()
{
}

void CObject3D::SetName(std::string name)
{
	this->Name = name;
}

std::string CObject3D::GetName()
{
	return this->Name;
}

void CObject3D::AddComponent(int id, std::string name)
{
	switch (id)
	{
		case(Object3DComponent::BASE_COMPONENT):
		{
			std::shared_ptr<CBaseComponent> temp=std::make_shared<CBaseComponent>();
			temp->SetName(name);
			temp->SetType(Object3DComponent::BASE_COMPONENT);
			//temp->SetPossesingObject(std::make_shared<CObject3D>(*this));
			this->_Components.push_back(temp);
			break;
		}

		case(Object3DComponent::STATIC_MESH_COMPONENT): 
		{
			std::shared_ptr<CStaticMeshComponent> temp = std::make_shared<CStaticMeshComponent>();
			temp->SetName(name);
			temp->SetType(Object3DComponent::STATIC_MESH_COMPONENT);
			temp->AttachParrentObject(this->GetRootComponent());
			//temp->SetPossesingObject(std::make_shared<CObject3D>(*this));
			this->_Components.push_back(temp);
			break;
		}
		case(Object3DComponent::CAMERA_COMPONENT): {
			std::shared_ptr<CCameraComponent> temp = std::make_shared<CCameraComponent>();
			temp->SetName(name);
			temp->SetType(Object3DComponent::CAMERA_COMPONENT);
			temp->AttachParrentObject(this->GetRootComponent());
			//temp->SetPossesingObject(std::make_shared<CObject3D>(*this));
			this->_Components.push_back(temp);
			break;
		}
		case(Object3DComponent::LIGHT_COMPONENT): {
			std::shared_ptr<CLightComponent> temp = std::make_shared<CLightComponent>();
			temp->SetName(name);
			temp->SetType(Object3DComponent::LIGHT_COMPONENT);
			temp->AttachParrentObject(this->GetRootComponent());
			//temp->SetPossesingObject(std::make_shared<CObject3D>(*this));
			this->_Components.push_back(temp);
			break;
		}
		case(Object3DComponent::MOVEMENT_COMPONENT): {
			std::shared_ptr<CMovementComponent> temp = std::make_shared<CMovementComponent>();
			temp->SetName(name);
			temp->SetType(Object3DComponent::MOVEMENT_COMPONENT);
			temp->AttachParrentObject(this->GetRootComponent());
			//temp->SetPossesingObject(std::make_shared<CObject3D>(*this));
			this->_Components.push_back(temp);
			break;
		}
	}
}

void CObject3D::RemoveComponent(std::string name)
{
	for(unsigned int i=0;i<this->_Components.size();i++)
	{
		std::shared_ptr<CBaseComponent> temp(this->_Components[i]);
		if (name == temp->GetName())
		{
			this->_Components.emplace(this->_Components.begin()+i);
			break;
		}
	}
}

void CObject3D::BindRootComponent(std::string name)
{
	for (auto o : this->_Components)
	{
		if (o->GetName() == name)
		{
			this->_RootComponent = o;
		}
	}
}

std::shared_ptr<CBaseComponent> CObject3D::GetRootComponent()
{
	return this->_RootComponent;
}

void CObject3D::SetPosition(glm::vec3 pos)
{
	this->_RootComponent->SetPosition(pos);
}

void CObject3D::SetRotation(glm::vec3 rot)
{
	this->_RootComponent->SetRotation(rot);
}

void CObject3D::SetScale(glm::vec3 scale)
{
	this->_RootComponent->SetScale(scale);
}

std::shared_ptr<CBaseComponent> CObject3D::GetComponentByType(Object3DComponent type)
{
	for (auto o : this->_Components)
	{
		if (o->GetType() == type)
		{
			return o;
		}
	}
	return nullptr;
}

std::vector<std::shared_ptr<CBaseComponent>> CObject3D::GetComponentList()
{
	return this->_Components;
}

void CObject3D::Tick(uint32_t delta)
{
	for (auto o : this->_Components)
	{
		o->Tick(delta);
	}
}
