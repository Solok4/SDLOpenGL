#include "pch.h"
#include "CCameraComponent.h"
#include "CEvent.h"
#include "CRenderer.h"
#include "COpengl.h"
#ifndef __EMSCRIPTEN__
#include <Windows.h>
#endif


CCameraComponent::CCameraComponent() 
{
	this->IsFree = false;
	this->FOV =65.f;
	this->Sensitivity = 5.f;
}

CCameraComponent::CCameraComponent(const CCameraComponent& cam):CBaseComponent(cam)
{
	this->LastX = cam.LastX;
	this->LastY = cam.LastY;
	this->FOV = cam.FOV;
	this->Sensitivity = cam.Sensitivity;
	this->IsFree = cam.IsFree;
	this->First = cam.First;
}

CCameraComponent::CCameraComponent(const std::shared_ptr<CCameraComponent>& camera) :CBaseComponent(camera)
{
	this->LastX = camera->LastX;
	this->LastY = camera->LastY;
	this->FOV = camera->FOV;
	this->Sensitivity = camera->Sensitivity;
	this->IsFree = camera->IsFree;
	this->First = camera->First;
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

void CCameraComponent::ProcessMouseMovements()
{
	if (this->IsFree)
	{


		int DeltaX = 0;
		int DeltaY = 0;
		int MouseX = 0;
		int MouseY = 0;
		Event->GetMouseMotion(MouseX, MouseY);
#ifdef __EMSCRIPTEN__
		DeltaX = MouseX - this->LastX;
		DeltaY = MouseY - this->LastY;
		this->LastX = MouseX;
		this->LastY = MouseY;
#else
		auto WNDInfo = Renderer->GetWindowInfo();
		int w = WNDInfo->ScreenWidth; int h = WNDInfo->ScreenHeight;
		int xpos = WNDInfo->ScreenPosX; int ypos = WNDInfo->ScreenPosY;
		/*SDL_GetWindowPosition(Wnd, &xpos, &ypos);
		SDL_GetWindowSize(Wnd, &w, &h);*/
		SetCursorPos(xpos + (w / 2), ypos + (h / 2));
		DeltaX = MouseX - (w / 2);
		DeltaY = MouseY - (h / 2);
#endif
		if (this->First)
		{
			this->LastX = MouseX;
			this->LastY = MouseY;
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

void CCameraComponent::Tick(double delta)
{
	this->_ParrentObject->SetRotation(this->_Rotation);
	CBaseComponent::Tick(delta);
	if (this->IsFree)
	{
		this->ProcessMouseMovements();
		OpenGL->GetShadersClass().SetCurrentShaderProgram("Default");
		glUniform3f(OpenGL->GetShadersClass().GetUniformByNameStruct("Default", "CameraPos"),
			this->_Position.x+this->_PosOffset.x, this->_Position.y+ this->_PosOffset.y, this->_Position.z+ this->_PosOffset.z);
	}
}
