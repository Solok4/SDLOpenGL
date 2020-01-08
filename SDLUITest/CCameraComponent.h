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
	//Sets field of view of the camera.
	void SetFov(float fov);
	//Returns field of view value.
	float GetFov();
	//Translate mouse position to camera movements.
	void ProcessMouseMovements();
	//Set camera into freelook mode.
	void SetIsFree(bool used);
	//Clip camera angles.
	void ClipCamera();
	//Function called every frame.
	void Tick(double delta) override;

private:

	int LastX = 0;
	int LastY = 0;
	float FOV;
	float Sensitivity = 5;
	bool IsFree = false;
	bool First = true;
};

