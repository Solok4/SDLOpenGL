#include "pch.h"
#include "CScene.h"



CScene::CScene()
{
}


CScene::~CScene()
{
	CLog::MyLog(0, "SceneDestructor "+this->GetName());
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
			this->Objects3D.emplace(this->Objects3D.begin()+i);
			break;
		}
	}
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

void CScene::SetCamera(std::shared_ptr<CCameraComponent> Cam)
{
	this->Camera = Cam;
}

std::shared_ptr<CCameraComponent> CScene::GetCamera()
{
	return this->Camera;
}
