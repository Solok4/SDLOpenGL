#pragma once

#include "glm/glm.hpp"
#include "CLog.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include <memory>
#include <cmath>

enum Object3DComponent
{
	BASE_COMPONENT = 0,
	STATIC_MESH_COMPONENT,
	CAMERA_COMPONENT,
	LIGHT_COMPONENT,
	MOVEMENT_COMPONENT,
};

class CObject3D;

class CBaseComponent
{
public:
	CBaseComponent();
	CBaseComponent(const CBaseComponent& comp);
	CBaseComponent(const std::shared_ptr<CBaseComponent>& obj);
	~CBaseComponent();
	void SetName(std::string name);
	std::string GetName();
	void AttachParrentObject(std::shared_ptr<CBaseComponent> Parrent);
	void SetPossesingObject(std::shared_ptr<CObject3D> obj);
	std::shared_ptr<CObject3D> GetPossesingObject();
	void SetType(Object3DComponent type);
	Object3DComponent GetType();

	void CalculateMatrix();
	glm::vec3 GetForwardVector();
	glm::vec3 GetRightVector();
	glm::vec3 GetUpVector();
	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void SetScale(glm::vec3 scale);

	virtual void Tick(uint32_t delta);


	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
	glm::mat4 GetModelMatrix();

protected:

	std::shared_ptr<CBaseComponent> _ParrentObject;
	std::shared_ptr<CObject3D> PossesingObject;
	std::string _Name;
	Object3DComponent _Type;

	glm::vec3 _Position;
	glm::vec3 _Scale;
	glm::vec3 _Rotation;
	glm::vec3 _PosOffset;
	glm::vec3 _RotOffset;

	glm::mat4 ModelMatrix;

	bool _IsActive = true;

};

