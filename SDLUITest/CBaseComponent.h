#pragma once

#include "glm.hpp"
#include "CObject3D.h" 
#include "CLog.h"

using namespace glm;

class CObject3D;

class CBaseComponent
{
public:
	CBaseComponent();
	~CBaseComponent();

	virtual void CreateComponent() = 0;
	virtual void AttachToObject(CObject3D *obj) = 0;
	virtual void DetachFromObject(CObject3D *obj) = 0;
	virtual void FreeComponent() = 0;

protected:
	unsigned int _ID;
	unsigned int _Type; 

	vec3 _Position;
	vec3 _Scale;
	vec3 _Rotation;

	bool _IsVisible = true;
	bool _IsActive = true;

};

