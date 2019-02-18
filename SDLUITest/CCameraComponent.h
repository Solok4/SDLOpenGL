#pragma once
#include "CBaseComponent.h"
#include "SDL.h"
class CCameraComponent :
	public CBaseComponent
{
public:
	CCameraComponent();
	~CCameraComponent();

	void SetFov(float fov);
	float GetFov();
	void ProcessMouseMovements(SDL_MouseButtonEvent e, SDL_Window* Wnd);
	void SetIsFree(bool used);
	void ClipCamera();


	

private:

	float FOV;
	float Sensitivity = 5;
	bool IsFree = false;
	bool First = true;
};

