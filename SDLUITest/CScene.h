#pragma once

#include <vector>
#include <memory>
#include "CObject3D.h"

class CScene
{
public:
	CScene();
	~CScene();

	void AddObjectToScene(std::string Name);
	std::shared_ptr<CObject3D> GetObjectByName(std::string Name);
	void RemoveObjectFromScene(std::string Name);
	void SetName(std::string Name);
	std::string GetName();
	void Draw(COpengl* opengl);
	void Tick(uint32_t delta);

	void Prepare();

	void SetCamera(std::shared_ptr<CCameraComponent> Cam);
	std::shared_ptr<CCameraComponent> GetCamera();

	std::vector <std::shared_ptr<CObject3D>> Objects3D;
	std::string Name;
	std::shared_ptr<CCameraComponent> Camera;

};

