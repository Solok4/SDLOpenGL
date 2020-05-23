#pragma once

#include "glm/glm.hpp"
#include <map>
#include <memory>
#include "CStaticMeshComponent.h"
#include "CCameraComponent.h"
#include "CLightComponent.h"
#include "CMovementComponent.h"
#include "COpengl.h"

class CScene;

class CObject3D
{
public:
	CObject3D(std::string name);
	CObject3D(std::string name, CScene* ref);
	CObject3D(const CObject3D& obj,std::string name, CScene* ref);
	~CObject3D();

	virtual void Prepare();
	//Sets name of the object.
	void SetName(std::string name);
	//Returns name of the object.
	std::string GetName();
	//Adds component based on id.
	void AddComponent(int id,std::string name);
	//Adds component based on another component.
	//template<typename T>
	void AddComponent(std::shared_ptr<CBaseComponent> comp);
	//Removes component from the object.
	void RemoveComponent(std::string name);
	//Returns component by name and type.
	template<typename T>
	std::shared_ptr<T> GetComponentByName(std::string name)
	{
		for (std::shared_ptr<CBaseComponent> o : this->_Components)
		{
			if (o->GetName() == name)
			{
				return std::dynamic_pointer_cast<T>(o);
			}
		}
		return nullptr;
	};
	//Bind component as the main in whole object.
	void BindRootComponent(std::string name);
	//Returns main component
	std::shared_ptr<CBaseComponent> GetRootComponent();
	//Sets position of the main component.
	void SetPosition(glm::vec3 pos);
	//Sets rotation of the main component.
	void SetRotation(glm::vec3 rot);
	//Sets scale of the main component.
	void SetScale(glm::vec3 scale);
	//Returns component list based on the type.
	std::shared_ptr<CBaseComponent> GetComponentByType(Object3DComponent type);
	//Returns all components
	std::vector<std::shared_ptr<CBaseComponent>> GetComponentList();
	//Called every frame.
	void Tick(double delta);


protected:

	std::string Name;
	std::vector<std::shared_ptr<CBaseComponent>> _Components;
	std::shared_ptr<CBaseComponent> _RootComponent;
	CScene* SceneRef;
};

