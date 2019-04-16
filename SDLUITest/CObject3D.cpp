#include "pch.h"
#include "CObject3D.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

CObject3D::CObject3D()
{
	this->AddComponent(0, "root");
	this->BindRootComponent("root");
}


CObject3D::~CObject3D()
{
	CLog::MyLog(0, "3DObjectDestructor %s",this->GetName().c_str());
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
			temp->CalculateMatrix();
			opengl->SetModelMatrix(temp->GetModelMatrix());
			opengl->SetNormalMatrix(temp->GetModelMatrix());
			glUniform3f(opengl->GetShadersClass().GetUniformByNameStruct("Default", "Mat.Ambient"),
				temp->GetModel()->Mat->LM.Ambient.x, temp->GetModel()->Mat->LM.Ambient.y, temp->GetModel()->Mat->LM.Ambient.z);
			glUniform3f(opengl->GetShadersClass().GetUniformByNameStruct("Default", "Mat.Diffuse"),
				temp->GetModel()->Mat->LM.Diffuse.x, temp->GetModel()->Mat->LM.Diffuse.y, temp->GetModel()->Mat->LM.Diffuse.z);
			glUniform3f(opengl->GetShadersClass().GetUniformByNameStruct("Default", "Mat.Specular"),
				temp->GetModel()->Mat->LM.Specular.x, temp->GetModel()->Mat->LM.Specular.y, temp->GetModel()->Mat->LM.Specular.z);
			glUniform1f(opengl->GetShadersClass().GetUniformByNameStruct("Default", "Mat.Shininess"),
				temp->GetModel()->Mat->LM.Shininess);
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
	switch (id)
	{
		case(Object3DComponent::BASE_COMPONENT):
		{
			std::shared_ptr<CBaseComponent> temp(new CBaseComponent);
			temp->SetName(name);
			temp->SetType(Object3DComponent::BASE_COMPONENT);
			//temp->SetPossesingObject(std::make_shared<CObject3D>(this));
			this->_Components.push_back(temp);
			break;
		}

		case(Object3DComponent::STATIC_MESH_COMPONENT): 
		{
			std::shared_ptr<CStaticMeshComponent> temp(new CStaticMeshComponent);
			temp->SetName(name);
			temp->SetType(Object3DComponent::STATIC_MESH_COMPONENT);
			temp->AttachParrentObject(this->GetRootComponent());
			//temp->SetPossesingObject(std::make_shared<CObject3D>(this));
			this->_Components.push_back(temp);
			break;
		}
		case(Object3DComponent::CAMERA_COMPONENT): {
			std::shared_ptr<CCameraComponent> temp(new CCameraComponent);
			temp->SetName(name);
			temp->SetType(Object3DComponent::CAMERA_COMPONENT);
			temp->AttachParrentObject(this->GetRootComponent());
			//temp->SetPossesingObject(std::make_shared<CObject3D>(this));
			this->_Components.push_back(temp);
			break;
		}
		case(Object3DComponent::LIGHT_COMPONENT): {
			std::shared_ptr<CLightComponent> temp(new CLightComponent);
			temp->SetName(name);
			temp->SetType(Object3DComponent::LIGHT_COMPONENT);
			temp->AttachParrentObject(this->GetRootComponent());
			//temp->SetPossesingObject(std::make_shared<CObject3D>(this));
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

std::shared_ptr<CBaseComponent> CObject3D::GetComponentByType(int type)
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

void CObject3D::Tick(uint32_t delta)
{
	for (auto o : this->_Components)
	{
		o->Tick(delta);
	}
}
