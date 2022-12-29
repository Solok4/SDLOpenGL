#pragma once

#include <vector>
#include <memory>

#include "../components/CObject3D.h"
#include "../renderer/IDraw.h"
#include "../components/CHeightMapComponent.h"
#include "CSkybox.h"

class CScene
{
public:
	CScene();
	~CScene();

	std::shared_ptr<CObject3D> AddObjectToScene(std::string Name);
	std::shared_ptr<CObject3D> AddObjectToScene(std::string Name, std::shared_ptr<CObject3D>& obj);
	std::shared_ptr<CObject3D> GetObjectByName(std::string Name);
	void RemoveObjectFromScene(std::string Name);
	void AddLightToScene(std::shared_ptr<CObject3D> light);
	void AddLightToScene(std::string Name);
	void RemoveLightFromScene(std::shared_ptr<CObject3D> light);
	void RemoveLightFromScene(std::string Name);
	std::vector<std::shared_ptr<CLightComponent>> GetLightObjects();
	void SetName(std::string Name);
	std::string GetName();
	void Draw(DrawType);

	void Tick(double delta);

	void Prepare();

	void ProcessLights();

	void SetCamera(std::shared_ptr<CCameraComponent> Cam);
	void SetCamera(std::shared_ptr<CObject3D> Camera);
	std::shared_ptr<CCameraComponent> GetCamera();

	void SetMovementObject(std::shared_ptr<CMovementComponent> Movement);
	void SetMovementObject(std::shared_ptr<CObject3D> Movement);
	std::shared_ptr<CMovementComponent> GetMovementObject();

	void CacheObjectsToDraw();

private:
	bool DrawableCached = false;
	std::vector <std::shared_ptr<IDraw>> ObjectsToDraw;

	std::vector <std::shared_ptr<CObject3D>> Objects3D;
	std::vector <std::shared_ptr<CLightComponent>> Lights;
	std::string Name;

	std::shared_ptr<CCameraComponent> Camera;
	std::shared_ptr<CMovementComponent> MovementObject;
	std::shared_ptr<CSkybox> Skybox;
	std::shared_ptr<CHeightMapComponent> _HeightMap;
};
