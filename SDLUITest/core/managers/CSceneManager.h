#pragma once
#include "../../CScene.h"
#include "../components/CCameraComponent.h"
#include <vector>

class CSceneManager
{
public:
	CSceneManager();
	~CSceneManager();

	void AddNewScene(std::string Name);
	std::shared_ptr<CScene> GetSceneByName(std::string Name);
	void RemoveScene(std::string Name);
	std::shared_ptr<CScene> GetCurrentScene();
	void SetCurrentScene(std::string Name);

	std::vector<std::shared_ptr<CScene>> Scenes;
	std::shared_ptr<CScene> CurrentScene;
};

extern std::unique_ptr<CSceneManager> SceneManager;
