#include "pch.h"
#include "CScene.h"



CScene::CScene()
{
	this->Camera = nullptr;
	this->MovementObject = nullptr;
}


CScene::~CScene()
{
	CLog::MyLog(0, "SceneDestructor %s",this->GetName().c_str());
}

void CScene::AddObjectToScene(std::string Name)
{
	std::shared_ptr<CObject3D> temp(new CObject3D);
	temp->SetName(Name);
	this->Objects3D.push_back(temp);
}

std::shared_ptr<CObject3D> CScene::GetObjectByName(std::string Name)
{
	for (auto o : this->Objects3D)
	{
		if (o->GetName() == Name)
		{
			return o;
		}
	}

	return nullptr;
}

void CScene::RemoveObjectFromScene(std::string Name)
{
	for (unsigned int i = 0; i < this->Objects3D.size(); i++)
	{
		if (this->Objects3D[i]->GetName() == Name)
		{
			this->Objects3D.erase(this->Objects3D.begin()+i);
			break;
		}
	}
}

void CScene::AddLightToScene(std::shared_ptr<CObject3D> light)
{
	if (light != nullptr)
	{
		if(light->GetComponentByType(Object3DComponent::LIGHT_COMPONENT) != nullptr)
			this->Lights.push_back(std::dynamic_pointer_cast<CLightComponent>(light->GetComponentByType(Object3DComponent::LIGHT_COMPONENT)));
	}
}

void CScene::AddLightToScene(std::string Name)
{
	for (auto o : this->Objects3D)
	{
		if (o->GetName() == Name)
		{
			if (o->GetComponentByType(Object3DComponent::LIGHT_COMPONENT) != nullptr)
			{
				this->Lights.push_back(std::dynamic_pointer_cast<CLightComponent>(o->GetComponentByType(Object3DComponent::LIGHT_COMPONENT)));
			}
		}
	}
}

void CScene::RemoveLightFromScene(std::shared_ptr<CObject3D> light)
{
	if (light != nullptr)
	{
		if (light->GetComponentByType(Object3DComponent::LIGHT_COMPONENT) != nullptr)
		{
			for (unsigned int i = 0; i < this->Lights.size(); i++)
			{
				if (this->Lights[i]->GetName() == light->GetName())
				{
					this->Lights.erase(this->Lights.begin() + i);
					break;
				}
			}
		}
	}
	CScene::RemoveObjectFromScene(light->GetName());
}

void CScene::RemoveLightFromScene(std::string Name)
{
	for (unsigned int i = 0; i < this->Lights.size(); i++)
	{
		if (this->Lights[i]->GetName() == Name)
		{
			this->Lights.erase(this->Lights.begin() + i);
			break;
		}
	}
	CScene::RemoveObjectFromScene(Name);
}

std::vector<std::shared_ptr<CLightComponent>> CScene::GetLightObjects()
{
	return this->Lights;
}


void CScene::SetName(std::string Name)
{
	this->Name = Name;
}

std::string CScene::GetName()
{
	return this->Name;
}

void CScene::Draw(COpengl * opengl)
{

	for (auto o : Objects3D)
	{
		o->Draw(opengl);
	}
}

void CScene::Tick(uint32_t delta)
{
	for (auto o : this->Objects3D)
	{
		o->Tick(delta);
	}
}

void CScene::Prepare()
{
}

void CScene::SetCamera(std::shared_ptr<CCameraComponent> Cam)
{
	if (Cam != nullptr)
	{
		this->Camera = Cam;
		return;
	}
	CLog::MyLog(1, "Camera not bound to scene: %s ", this->Name.c_str());
}

std::shared_ptr<CCameraComponent> CScene::GetCamera()
{
	return this->Camera;
}

void CScene::SetMovementObject(std::shared_ptr<CMovementComponent> Movement)
{
	if (Movement != nullptr)
	{
		this->MovementObject = Movement;
	}
}

std::shared_ptr<CMovementComponent> CScene::GetMovementObject()
{
	return this->MovementObject;
}
