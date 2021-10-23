#pragma once
#include "CBaseComponent.h"

class CMovementComponent :
	public CBaseComponent
{
public:
	CMovementComponent(CObject3D* ref);
	CMovementComponent(const CMovementComponent& move, CObject3D* ref);
	//CMovementComponent(const std::shared_ptr<CMovementComponent>& move);
	~CMovementComponent();

	void MoveForward(float isForward);
	void MoveHorizontal(float isRight);
	void MoveVertical(float isUp);

	void Tick(double delta) override;

	double MaxAccel;
	double MaxSpeed;
	double CurrentXAccel;
	double CurrentYAccel;
	double CurrentZAccel;
	double CurrentSpeed;
	double AccelTime;
	double SlowDownFactor;

	float isMoveForward = 0.0f;
	float isMoveRight = 0.0f;
	float isMoveUp = 0.0f;
	glm::vec3 ResultVector;
};
