#pragma once
#include "CBaseComponent.h"
class CCameraComponent :
	public CBaseComponent
{
public:
	CCameraComponent();
	~CCameraComponent();

	void SetFov(float fov);
	float GetFov();
	

private:

	float FOV;
};

