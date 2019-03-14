#include "pch.h"
#include "CCameraComponent.h"
#include <Windows.h>


CCameraComponent::CCameraComponent() 
{
	this->IsFree = false;
	this->FOV =65.f;
	this->Sensitivity = 5.f;
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

void CCameraComponent::ProcessMouseMovements(SDL_MouseButtonEvent e, SDL_Window* Wnd)
{
	if (this->IsFree)
	{

		int w = 0; int h = 0;
		int xpos = 0; int ypos = 0;
		SDL_GetWindowPosition(Wnd, &xpos, &ypos);
		SDL_GetWindowSize(Wnd, &w, &h);

		SetCursorPos(xpos + (w / 2), ypos + (h / 2));

		int DeltaX = 0;
		DeltaX = e.x - (w / 2);
		int DeltaY = 0;
		DeltaY = e.y - (h / 2);

		if (this->First)
		{
			DeltaX = 0;
			DeltaY = 0;
			this->First = false;
		}

		float MovementX = (DeltaX * this->Sensitivity)*0.1f;
		float MovementY = (DeltaY * this->Sensitivity)*0.1f;

		this->_Rotation.y += MovementX;
		this->_Rotation.x -= MovementY;
		this->ClipCamera();
		this->CalculateMatrix();
	}

	//CLog::MyLog(0, "X: %f Y: %f DeltaX: %d DeltaY: %d ", this->_Rotation.x, this->_Rotation.y, DeltaX, DeltaY);
	//CLog::MyLog(0, "MouseX: %d MouseY: %d Window W: %d Window H: %d\n",e.x,e.y,w,h);
	//CLog::MyLog(0, "ForwardVector X: %f Y: %f Z: %f", this->GetForwardVector().x, this->GetForwardVector().y, this->GetForwardVector().z);
}

void CCameraComponent::SetIsFree(bool used)
{
	if (this->IsFree != used)
	{
		this->IsFree = used;
		this->First = true;
	}
}

void CCameraComponent::ClipCamera()
{
	if (this->_Rotation.x > 89)
	{
		this->_Rotation.x = 89;
	}
	else if (this->_Rotation.x < -89)
	{
		this->_Rotation.x = -89;
	}

	if (this->_Rotation.y > 180)
	{
		this->_Rotation.y = -179;
	}
	else if (this->_Rotation.y < -179)
	{
		this->_Rotation.y = 180;
	}

	if (this->_Rotation.z > 180)
	{
		this->_Rotation.z = -179;
	}
	else if (this->_Rotation.z < -179)
	{
		this->_Rotation.z = 180;
	}
}
