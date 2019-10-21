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
	CObject3D(const CObject3D& obj);
	CObject3D(const std::shared_ptr<CObject3D>& obj);
	~CObject3D();

	virtual void Prepare();
	void SetName(std::string name);
	std::string GetName();
	void AddComponent(int id,std::string name);
	template<typename T>
	void AddComponent(std::shared_ptr<T> comp)
	{
		std::shared_ptr<T> newComp = std::make_shared<T>(comp);
		std::shared_ptr<CBaseComponent> fix = static_cast<std::shared_ptr<CBaseComponent>>(newComp);
		if (fix == nullptr)
		{
			CLog::MyLog(LogType::Error, "Wrong component provided to add");
			return;
		}
		fix->AttachParrentObject(this->GetRootComponent());
		this->_Components.push_back(newComp);
	}
	void RemoveComponent(std::string name);

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

	void BindRootComponent(std::string name);
	std::shared_ptr<CBaseComponent> GetRootComponent();
	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void SetScale(glm::vec3 scale);
	std::shared_ptr<CBaseComponent> GetComponentByType(Object3DComponent type);
	std::vector<std::shared_ptr<CBaseComponent>> GetComponentList();

	void Tick(uint32_t delta);


protected:

	//std::shared_ptr<CObject3D> _Pointer;
	std::string Name;
	std::vector<std::shared_ptr<CBaseComponent>> _Components;
	std::shared_ptr<CBaseComponent> _RootComponent;
};

