#pragma once

#include "glm.hpp"
#include "CLog.h"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"

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

	void SetType(int type);
	int GetType();

	void CalculateMatrix();
	glm::vec3 GetForwardVector();
	glm::vec3 GetRightVector();
	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void SetScale(glm::vec3 scale);

	glm::vec3 GetPosition();
	glm::mat4 GetModelMatrix();

	virtual void Draw();

protected:

	std::shared_ptr<CBaseComponent> _ParrentObject;
	std::string _Name;
	int _Type;

	glm::vec3 _Position;
	glm::vec3 _Scale;
	glm::vec3 _Rotation;

	glm::mat4 ModelMatrix;

	bool _IsActive = true;

};

