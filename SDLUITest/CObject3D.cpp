#include "pch.h"
#include "CObject3D.h"
#include "gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"

CObject3D::CObject3D()
{
	this->AddComponent(0, "root");
}


CObject3D::~CObject3D()
{
}

void CObject3D::Prepare()
{
}

void CObject3D::Draw(COpengl * opengl)
{
	for (std::shared_ptr<CBaseComponent> o : this->_Components)
	{
		if (o->GetType() == Object3DComponent::STATIC_MESH_COMPONENT)
		{
			std::shared_ptr<CStaticMeshComponent> temp = std::dynamic_pointer_cast<CStaticMeshComponent>(o);
			opengl->SetModelMatrix(temp->GetModelMatrix());
			temp->Draw();
		}
	}
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
	if (id == Object3DComponent::BASE_COMPONENT)
	{
		std::shared_ptr<CBaseComponent> temp(new CBaseComponent);
		temp->SetName(name);
		temp->SetType(Object3DComponent::BASE_COMPONENT);
		this->_Components.push_back(temp);
	}
	else if (id == Object3DComponent::STATIC_MESH_COMPONENT)
	{
		std::shared_ptr<CStaticMeshComponent> temp(new CStaticMeshComponent);
		temp->SetName(name);
		temp->SetType(Object3DComponent::STATIC_MESH_COMPONENT);
		this->_Components.push_back(temp);
	}
}

void CObject3D::RemoveComponent(std::string name)
{
	for(int i=0;i<this->_Components.size();i++)
	{
		std::shared_ptr<CBaseComponent> temp(this->_Components[i]);
		if (name == temp->GetName())
		{
			this->_Components.emplace(this->_Components.begin()+i);
			break;
		}
	}
}

std::shared_ptr<CBaseComponent> CObject3D::GetComponentByName(std::string name)
{
	for (std::shared_ptr<CBaseComponent> o : this->_Components)
	{
		if (o->GetName() == name)
		{
			return o;
		}
	}
	return nullptr;
}
