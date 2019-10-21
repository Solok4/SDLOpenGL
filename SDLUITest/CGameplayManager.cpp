#include "pch.h"
#include "CGameplayManager.h"


CGameplayManager::CGameplayManager()
{
}


CGameplayManager::~CGameplayManager()
{
}

std::shared_ptr<CGameplay> CGameplayManager::AddNewGameplay(const char* name)
{
	std::shared_ptr<CGameplay> temp = std::make_shared<CGameplay>(name);
	this->GameplayConfigurations.push_back(temp);
	return temp;
}

void CGameplayManager::DeleteGameplay(const char* name)
{
	for (int i = 0; i < this->GameplayConfigurations.size(); i++)
	{
		if (strcmp(this->GameplayConfigurations[i]->Name, name) == 0)
		{
			this->GameplayConfigurations.erase(this->GameplayConfigurations.begin() + i);
			return;
		}
	}
	CLog::MyLog(LogType::Error, "Gameplay configuration named %s not found", name);
}

void CGameplayManager::SelectCurrentGameplay(const char* name)
{
	for (auto a : this->GameplayConfigurations)
	{
		if (strcmp(a->Name, name) == 0)
		{
			this->CurrentGameplay = a;
			return;
		}
	}
	CLog::MyLog(LogType::Error, "Gameplay configuration named %s not found", name);
}

std::shared_ptr<CGameplay> CGameplayManager::GetCurrentGameplay()
{
	return this->CurrentGameplay;
}
