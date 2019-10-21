#pragma once
#include <vector>
#include <memory>
#include "CGameplay.h"
#include "CLog.h"

class CGameplayManager
{
public:
	CGameplayManager();
	~CGameplayManager();

	std::shared_ptr<CGameplay> AddNewGameplay(const char* name);
	void DeleteGameplay(const char* name);
	void SelectCurrentGameplay(const char* name);
	std::shared_ptr<CGameplay> GetCurrentGameplay();
	


	std::vector<std::shared_ptr<CGameplay>> GameplayConfigurations;

	std::shared_ptr<CGameplay> CurrentGameplay;
};

