#pragma once
#include "CBaseComponent.h"
class CMovementComponent :
	public CBaseComponent
{
public:
	CMovementComponent(CObject3D* ref);
	CMovementComponent(const CMovementComponent& move,CObject3D* ref);
	//CMovementComponent(const std::shared_ptr<CMovementComponent>& move);
	~CMovementComponent();

	void MoveForward(bool isForward);
	void MoveBackwards(bool isBackward);
	void MoveRight(bool isRight);
	void MoveLeft(bool isLeft);
	void MoveUp(bool isUp);
	void MoveDown(bool isDown);

	void Tick(double delta) override;

	double MaxAccel =0.1f;
	double MaxSpeed =1;
	double CurrentXAccel= 0;
	double CurrentYAccel = 0;
	double CurrentZAccel = 0;
	double CurrentSpeed = 0;
	double AccelTime=200;

	bool isMoveForward = false;
	bool isMoveBackward = false;
	bool isMoveRight = false;
	bool isMoveLeft = false;
	bool isMoveUp = false;
	bool isMoveDown = false;
	glm::vec3 ResultVector;
};

