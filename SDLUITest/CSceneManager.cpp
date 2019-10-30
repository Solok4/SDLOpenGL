#include "pch.h"
#include "CSceneManager.h"

std::unique_ptr<CSceneManager> SceneManager;

CSceneManager::CSceneManager()
{
	this->AddNewScene("Blank");
	this->SetCurrentScene("Blank");
}


CSceneManager::~CSceneManager()
{
	CLog::MyLog(LogType::Log, "SceneManagerDestructor");
}

void CSceneManager::AddNewScene(std::string Name)
{
	std::shared_ptr<CScene> temp(new CScene);
	temp->SetName(Name);
	this->Scenes.push_back(temp);
}

std::shared_ptr<CScene> CSceneManager::GetSceneByName(std::string Name)
{
	for (auto &o : this->Scenes)
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
	for (unsigned int i = 0; i < this->Scenes.size(); i++)
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
			this->Camera = o->GetCamera();
			break;
		}
	}
}

void CSceneManager::SetCamera(std::shared_ptr<CCameraComponent> Cam)
{
	if (Cam != nullptr)
	{
		this->Camera = Cam;
		return;
	}
	CLog::MyLog(LogType::Error, "Camera not bound to scene: %s ",this->CurrentScene->GetName().c_str());

}

std::shared_ptr<CCameraComponent> CSceneManager::GetCamera()
{
	return this->Camera;
}
