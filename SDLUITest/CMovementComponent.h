#pragma once
#include "CBaseComponent.h"
class CMovementComponent :
	public CBaseComponent
{
public:
	CMovementComponent();
	~CMovementComponent();

	void MoveForward(bool isForward);
	void MoveBackwards(bool isBackward);
	void MoveRight(bool isRight);
	void MoveLeft(bool isLeft);

	void Tick(uint32_t delta) override;

	float MaxAccel =0.1f;
	float MaxSpeed =1;
	float CurrentXAccel=0;
	float CurrentYAccel = 0;
	float CurrentSpeed = 0;
	float AccelTime=300;

	bool isMoveForward = false;
	bool isMoveBackward = false;
	bool isMoveRight = false;
	bool isMoveLeft = false;
	glm::vec3 ResultVector;
};

