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
	void AddLightToScene(std::shared_ptr<CObject3D> light);
	void AddLightToScene(std::string Name);
	void RemoveLightFromScene(std::shared_ptr<CObject3D> light);
	void RemoveLightFromScene(std::string Name);
	std::vector<std::shared_ptr<CLightComponent>> GetLightObjects();
	void SetName(std::string Name);
	std::string GetName();
	void Draw(COpengl* opengl);
	void Tick(uint32_t delta);

	void Prepare();

	void SetCamera(std::shared_ptr<CCameraComponent> Cam);
	std::shared_ptr<CCameraComponent> GetCamera();

	void SetMovementObject(std::shared_ptr<CMovementComponent> Movement);
	std::shared_ptr<CMovementComponent> GetMovementObject();

	std::vector <std::shared_ptr<CObject3D>> Objects3D;
	std::vector <std::shared_ptr<CLightComponent>> Lights;
	std::string Name;
	std::shared_ptr<CCameraComponent> Camera;
	std::shared_ptr<CMovementComponent> MovementObject;

};

