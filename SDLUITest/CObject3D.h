#pragma once

#include "glm/glm.hpp"
#include <map>
#include <memory>
#include "CStaticMeshComponent.h"
#include "CCameraComponent.h"
#include "CLightComponent.h"
#include "CMovementComponent.h"
#include "COpengl.h"


class CObject3D
{
public:
	CObject3D();
	~CObject3D();

	virtual void Prepare();
	void Draw(COpengl* opengl);
	void SetName(std::string name);
	std::string GetName();
	void AddComponent(int id,std::string name);
	void RemoveComponent(std::string name);
	std::shared_ptr<CBaseComponent> GetComponentByName(std::string name);
	void BindRootComponent(std::string name);
	std::shared_ptr<CBaseComponent> GetRootComponent();
	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void SetScale(glm::vec3 scale);
	std::shared_ptr<CBaseComponent> GetComponentByType(int type);

	void Tick(uint32_t delta);


protected:

	//std::shared_ptr<CObject3D> _Pointer;
	std::string Name;
	std::vector<std::shared_ptr<CBaseComponent>> _Components;
	std::shared_ptr<CBaseComponent> _RootComponent;
};

enum Object3DComponent
{
	BASE_COMPONENT = 0,
	STATIC_MESH_COMPONENT,
	CAMERA_COMPONENT,
	LIGHT_COMPONENT,
	MOVEMENT_COMPONENT,
};
