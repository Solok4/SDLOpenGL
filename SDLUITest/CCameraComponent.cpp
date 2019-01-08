#include "pch.h"
#include "CCameraComponent.h"


CCameraComponent::CCameraComponent()
{
}


CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::SetFov(float fov)
{
	if (fov > 0 && fov < 180)
	{
		this->FOV = fov;
	}
}

float CCameraComponent::GetFov()
{
	return this->FOV;
}
