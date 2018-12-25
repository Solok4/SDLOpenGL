#include "pch.h"
#include "CSceneManager.h"


CSceneManager::CSceneManager()
{
}


CSceneManager::~CSceneManager()
{
}

void CSceneManager::AddNewScene(std::string Name)
{
	std::shared_ptr<CScene> temp(new CScene);
	temp->SetName(Name);
	this->Scenes.push_back(temp);
}

std::shared_ptr<CScene> CSceneManager::GetSceneByName(std::string Name)
{
	for (auto o : this->Scenes)
	{
		if (o->GetName() == Name)
		{
			return o;
		}
	}
	return nullptr;
}

void CSceneManager::RemoveScene(std::string Name)
{
	for (int i = 0; i < this->Scenes.size(); i++)
	{
		if (this->Scenes[i]->GetName() == Name)
		{
			this->Scenes.emplace(this->Scenes.begin() + i);
			break;
		}
	}
}

std::shared_ptr<CScene> CSceneManager::GetCurrentScene()
{
	return this->CurrentScene;
}

void CSceneManager::SetCurrentScene(std::string Name)
{
	for (auto o : this->Scenes)
	{
		if (o->GetName() == Name)
		{
			this->CurrentScene = o;
			break;
		}
	}
}
