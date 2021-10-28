#pragma once

#include "../../glm/glm.hpp"
#include "../../CLog.h"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtx/transform.hpp"

#include <memory>
#include <cmath>
#include <string>

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
	CBaseComponent() = delete;
	CBaseComponent(CObject3D* ref);
	CBaseComponent(const CBaseComponent& comp, CObject3D* ref);
	~CBaseComponent();
	//Sets name of the component.
	void SetName(std::string name);
	//Returns name.
	std::string GetName();
	//Attach the component to another one.
	void AttachParrentObject(std::shared_ptr<CBaseComponent> Parrent);
	//Create reference to possesing object.
	void SetPossesingObject(CObject3D* obj);
	//Returns possesing object.
	CObject3D* GetPossesingObject();
	//Sets type of the component.
	void SetType(Object3DComponent type);
	//Returns type of the component.
	Object3DComponent GetType();
	//Calculate model matrix.
	void CalculateMatrix();
	//returns vector in front of the component.
	glm::vec3 GetForwardVector();
	//Returns vector to the right of the component.
	glm::vec3 GetRightVector();
	//Returns vector to the top of the component.
	glm::vec3 GetUpVector();
	//Returns if component is active.
	bool IsActive() { return this->_IsActive; };
	//Set component activity state.
	void SetIsActive(bool active) { this->_IsActive = active; };
	//Sets position of the component. If the component is a child of another one the position will be relative to parent component.
	void SetPosition(glm::vec3 pos);
	//Sets rotation of the component. If the component is a child of another one the rotation will be relative to parent component.
	void SetRotation(glm::vec3 rot);
	//Sets scale of the component. If the component is a child of another one the scale will be relative to parent component.
	void SetScale(glm::vec3 scale);
	//Function called every frame.
	virtual void Tick(double delta);
	//Returns vec3 position of the component.
	glm::vec3 GetPosition();
	//Returns vec3 rotation of the component.
	glm::vec3 GetRotation();
	//Returns vec3 scale of the component.
	glm::vec3 GetScale();
	//Returns model matrix.
	glm::mat4 GetModelMatrix();

protected:

	std::shared_ptr<CBaseComponent> _ParrentObject;
	CObject3D* PossesingObject;
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
