#include "pch.h"
#include "CGameplayManager.h"
#include "CKeyboardConf.h"
#include "CWindowManager.h"
std::unique_ptr<CGameplayManager> GameplayManager;

CGameplayManager::CGameplayManager()
{
}

CGameplayManager::~CGameplayManager()
{
}

std::shared_ptr<CGameplay> CGameplayManager::AddNewGameplay(const char* name)
{
	std::shared_ptr<CGameplay> temp = std::make_shared<CGameplay>(name);
	temp->Init();
	this->GameplayConfigurations.push_back(temp);
	return temp;
}

void CGameplayManager::DeleteGameplay(const char* name)
{
	for (int i = 0; i < this->GameplayConfigurations.size(); i++)
	{
		if (strcmp(this->GameplayConfigurations[i]->Name.c_str(), name) == 0)
		{
			this->GameplayConfigurations.erase(this->GameplayConfigurations.begin() + i);
			return;
		}
	}
	CLog::error("Gameplay configuration named %s not found", name);
}

void CGameplayManager::SelectCurrentGameplay(const char* name)
{
	for (auto a : this->GameplayConfigurations)
	{
		if (strcmp(a->Name.c_str(), name) == 0)
		{
			this->CurrentGameplay = a;
			KeyboardConf->ResetKeyFromTriggerStatus();
			a->OnGameplayChange();
			Renderer->SetFrameLock(a->GetFrameLimit());
			return;
		}
	}
	CLog::error("Gameplay configuration named %s not found", name);
}

std::shared_ptr<CGameplay> CGameplayManager::GetCurrentGameplay()
{
	return this->CurrentGameplay;
}