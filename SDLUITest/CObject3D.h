#pragma once

#include "glm.hpp"
#include <map>
#include <memory>
#include "CStaticMeshComponent.h"
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


protected:

	std::shared_ptr<CObject3D> _Pointer;
	std::string Name;
	std::vector<std::shared_ptr<CBaseComponent>> _Components;
};

enum Object3DComponent
{
	BASE_COMPONENT = 0,
	STATIC_MESH_COMPONENT,
};
