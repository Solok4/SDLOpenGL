#pragma once

#include "glm/glm.hpp"
#include "CLog.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include <cmath>

class CObject3D;

class CBaseComponent
{
public:
	CBaseComponent();
	~CBaseComponent();
	void SetName(std::string name);
	std::string GetName();
	void AttachParrentObject(std::shared_ptr<CBaseComponent> Parrent);
	void SetPossesingObject(std::shared_ptr<CObject3D> obj);
	std::shared_ptr<CObject3D> GetPossesingObject();
	void SetType(int type);
	int GetType();

	void CalculateMatrix();
	glm::vec3 GetForwardVector();
	glm::vec3 GetRightVector();
	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void SetScale(glm::vec3 scale);

	virtual void Tick(uint32_t delta);


	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
	glm::mat4 GetModelMatrix();

	virtual void Draw();

protected:

	std::shared_ptr<CBaseComponent> _ParrentObject;
	std::shared_ptr<CObject3D> PossesingObject;
	std::string _Name;
	int _Type;

	glm::vec3 _Position;
	glm::vec3 _Scale;
	glm::vec3 _Rotation;
	glm::vec3 _PosOffset;
	glm::vec3 _RotOffset;

	glm::mat4 ModelMatrix;

	bool _IsActive = true;

};

