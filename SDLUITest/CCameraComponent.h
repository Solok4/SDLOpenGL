#pragma once
#include "CBaseComponent.h"
#include "SDL.h"
class CCameraComponent :
	public CBaseComponent
{
public:
	CCameraComponent();
	CCameraComponent(const CCameraComponent& cam);
	CCameraComponent(const std::shared_ptr<CCameraComponent>& camera);
	~CCameraComponent();

	void SetFov(float fov);
	float GetFov();
	void ProcessMouseMovements();
	void SetIsFree(bool used);
	void ClipCamera();
	void Tick(double delta) override;



private:

	int LastX = 0;
	int LastY = 0;
	float FOV;
	float Sensitivity = 5;
	bool IsFree = false;
	bool First = true;
};

