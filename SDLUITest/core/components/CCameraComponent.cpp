#include "CCameraComponent.h"
#include "../managers/CEventManager.h"
#include "../managers/CWindowManager.h"
#include "../renderer/COpengl.h"
#include "CObject3D.h"
#include "../managers/CWindowManager.h"

CCameraComponent::CCameraComponent(CObject3D* ref) :CBaseComponent(ref)
{
	this->_Type = Object3DComponent::CAMERA_COMPONENT;
	this->IsFree = false;
	this->FOV = 65.f;
	this->Sensitivity = 5.f;
}

CCameraComponent::CCameraComponent(const CCameraComponent& cam, CObject3D* ref) :CBaseComponent(cam, ref)
{
	this->_Type = Object3DComponent::CAMERA_COMPONENT;
	this->LastX = cam.LastX;
	this->LastY = cam.LastY;
	this->FOV = cam.FOV;
	this->Sensitivity = cam.Sensitivity;
	this->IsFree = cam.IsFree;
	this->First = cam.First;
	this->AttachParrentObject(this->PossesingObject->GetRootComponent());
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
		EventManager->GetMouseMotion(MouseX, MouseY);
		SDL_ShowCursor(false);
		auto WNDInfo = WindowManager->GetWindowInfo();
		int w = WNDInfo->ScreenWidth; int h = WNDInfo->ScreenHeight;
		int xpos = WNDInfo->ScreenPosX; int ypos = WNDInfo->ScreenPosY;
		SDL_WarpMouseInWindow(WindowManager->GetWindow(),w / 2, h / 2);
		DeltaX = MouseX - (w / 2);
		DeltaY = MouseY - (h / 2);
		if (this->First)
		{
			this->LastX = MouseX;
			this->LastY = MouseY;
			DeltaX = 0;
			DeltaY = 0;
			this->First = false;
		}

		float MovementX = (DeltaX * this->Sensitivity) * 0.1f;
		float MovementY = (DeltaY * this->Sensitivity) * 0.1f;

		this->_Rotation.y -= MovementX;
		this->_Rotation.x -= MovementY;
		this->ClipCamera();
		this->CalculateMatrix();
	}
	else
	{
		SDL_ShowCursor(true);
	}

	//CLog::MyLog(LogType::Debug, "X: %f Y: %f", this->_Rotation.x, this->_Rotation.y);
	//CLog::MyLog(0, "MouseX: %d MouseY: %d Window W: %d Window H: %d\n",e.x,e.y,w,h);
	//CLog::MyLog(LogType::Debug, "ForwardVector X: %f Y: %f Z: %f", this->GetForwardVector().x, this->GetForwardVector().y, this->GetForwardVector().z);
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
	this->PerspectiveMatrix = glm::perspective(glm::radians(this->FOV), WindowManager->GetWindowInfo()->ScreenAspectRatio, 0.1f, 1000.f);
	this->ViewMatrix = glm::lookAt(this->_Position, this->_Position + this->GetForwardVector(), glm::vec3(0.f, 1.f, 0.f));
	this->ProcessMouseMovements();
	if (this->IsFree)
	{
		OpenGL->GetShadersClass()->SetCurrentShaderProgram("Default");
		glUniform3f(OpenGL->GetShadersClass()->GetUniformByNameStruct("Default", "CameraPos"),
			this->_Position.x + this->_PosOffset.x, this->_Position.y + this->_PosOffset.y, this->_Position.z + this->_PosOffset.z);
	}
}

glm::vec3 CCameraComponent::CastRay()
{
	int MouseX;
	int MouseY;
	EventManager->GetMouseMotion(MouseX, MouseY);

	float NormalizedX = (2.f * MouseX) / WindowManager->GetWindowInfo()->ScreenWidth - 1.f;
	float NormalizedY = 1.0f - (2.f * MouseY) / WindowManager->GetWindowInfo()->ScreenHeight;

	glm::vec3 NormalizedCords = glm::vec3(NormalizedX, NormalizedY, -1.f);

	glm::vec4 HomogenousClip = glm::vec4(NormalizedCords.x, NormalizedCords.y, -1.f, 1.f);

	glm::mat4 PerspectiveMatrix = glm::perspective(glm::radians(this->FOV), WindowManager->GetWindowInfo()->ScreenAspectRatio, 0.1f, 1000.f);
	glm::vec4 RayEyeSpace = glm::inverse(PerspectiveMatrix) * HomogenousClip;
	RayEyeSpace = glm::vec4(RayEyeSpace.x, RayEyeSpace.y, -1.f, 0.f);

	glm::mat4 ViewMatrix = glm::lookAt(this->_Position, this->_Position + this->GetForwardVector(), glm::vec3(0.0f, 1.f, 0.f));
	glm::vec3 RayWorld = (glm::inverse(ViewMatrix) * RayEyeSpace);
	this->LastRay = RayWorld;
	//CLog::MyLog(LogType::Debug, "RayCast x: %f y: %f z: %f", RayWorld.x, RayWorld.y, RayWorld.z);
	return RayWorld;
}