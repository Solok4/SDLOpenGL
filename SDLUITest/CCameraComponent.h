#pragma once
#include "CBaseComponent.h"
#include "SDL.h"
class CCameraComponent :
	public CBaseComponent
{
public:
	CCameraComponent(CObject3D* ref);
	CCameraComponent(const CCameraComponent& cam,CObject3D* ref);
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

	glm::mat4 GetPerspectiveMatrix() { return this->PerspectiveMatrix; };
	glm::mat4 GetViewMatrix() { return this->ViewMatrix; };

	glm::vec3 CastRay();

private:

	int LastX = 0;
	int LastY = 0;
	float FOV;
	float Sensitivity = 5;
	bool IsFree = false;
	bool First = true;

	glm::mat4 PerspectiveMatrix;
	glm::mat4 ViewMatrix;

	glm::vec3 LastRay;
};

