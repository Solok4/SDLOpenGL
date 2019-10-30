#include "pch.h"
#include "CMovementComponent.h"


CMovementComponent::CMovementComponent()
{
}

CMovementComponent::CMovementComponent(const CMovementComponent& move):CBaseComponent(move)
{
	this->MaxAccel = move.MaxAccel;
	this->MaxSpeed = move.MaxSpeed;
	this->CurrentXAccel = move.CurrentXAccel;
	this->CurrentYAccel = move.CurrentYAccel;
	this->CurrentZAccel = move.CurrentZAccel;
	this->AccelTime = move.AccelTime;
	this->isMoveBackward = move.isMoveBackward;
	this->isMoveDown = move.isMoveDown;
	this->isMoveForward = move.isMoveForward;
	this->isMoveLeft = move.isMoveLeft;
	this->isMoveRight = move.isMoveRight;
	this->isMoveUp = move.isMoveUp;
	this->ResultVector = move.ResultVector;
}

CMovementComponent::CMovementComponent(const std::shared_ptr<CMovementComponent>& move) :CBaseComponent(move)
{
	this->MaxAccel = move->MaxAccel;
	this->MaxSpeed = move->MaxSpeed;
	this->CurrentXAccel = move->CurrentXAccel;
	this->CurrentYAccel = move->CurrentYAccel;
	this->CurrentZAccel = move->CurrentZAccel;
	this->AccelTime = move->AccelTime;
	this->isMoveBackward = move->isMoveBackward;
	this->isMoveDown = move->isMoveDown;
	this->isMoveForward = move->isMoveForward;
	this->isMoveLeft = move->isMoveLeft;
	this->isMoveRight = move->isMoveRight;
	this->isMoveUp = move->isMoveUp;
	this->ResultVector = move->ResultVector;
}


CMovementComponent::~CMovementComponent()
{
}

void CMovementComponent::MoveForward(bool isForward)
{
	this->isMoveForward = isForward;
}

void CMovementComponent::MoveBackwards(bool isBackward)
{
	this->isMoveBackward = isBackward;
}

void CMovementComponent::MoveRight(bool isRight)
{
	this->isMoveRight = isRight;
}

void CMovementComponent::MoveLeft(bool isLeft)
{
	this->isMoveLeft = isLeft;
}

void CMovementComponent::MoveUp(bool isUp)
{
	this->isMoveUp = isUp;
}

void CMovementComponent::MoveDown(bool isDown)
{
	this->isMoveDown = isDown;
}


void CMovementComponent::Tick(uint32_t delta)
{
	CBaseComponent::Tick(delta);
	glm::vec3 diffX = this->GetForwardVector();
	glm::vec3 diffY = this->GetRightVector();
	glm::vec3 diffZ = this->GetUpVector();
	//CLog::MyLog(0, "FVector x:%f y:%f z:%f", this->GetForwardVector().x, this->GetForwardVector().y, this->GetForwardVector().z);
	if (isMoveForward)
	{
		if (this->CurrentXAccel < this->MaxAccel)
		{
			if (this->CurrentXAccel < 0)
				this->CurrentXAccel = 0;
			this->CurrentXAccel += (this->MaxAccel  * delta / this->AccelTime);	
		}		
	}
	if (isMoveBackward)
	{
		if (this->CurrentXAccel > 0)
			this->CurrentXAccel = 0;
		if (abs(this->CurrentXAccel) < this->MaxAccel)
			this->CurrentXAccel -= (this->MaxAccel  * delta / this->AccelTime);
	}
	if (isMoveRight)
	{
		if (this->CurrentYAccel < 0)
			this->CurrentYAccel = 0;
		if (this->CurrentYAccel < this->MaxAccel)
			this->CurrentYAccel += (this->MaxAccel  * delta / this->AccelTime);
	}
	if (isMoveLeft)
	{
		if (this->CurrentYAccel > 0)
			this->CurrentYAccel = 0;
		if (abs(this->CurrentYAccel) < this->MaxAccel)
			this->CurrentYAccel -= (this->MaxAccel  * delta / this->AccelTime);
		
	}
	if (isMoveUp)
	{
		if (this->CurrentZAccel < 0)
			this->CurrentZAccel = 0;
		if (this->CurrentZAccel < this->MaxAccel)
			this->CurrentZAccel += (this->MaxAccel * delta / this->AccelTime);
	}
	if (isMoveDown)
	{
		if (this->CurrentZAccel > 0)
			this->CurrentZAccel = 0;
		if (abs(this->CurrentZAccel) < this->MaxAccel)
			this->CurrentZAccel -= (this->MaxAccel * delta / this->AccelTime);

	}

	//SLOWDOWN
	if (!this->isMoveForward && !this->isMoveBackward)
		if (this->CurrentXAccel != 0)
		{
			if (this->CurrentXAccel > 0)
			{
				this->CurrentXAccel -= (this->MaxAccel  * delta / this->AccelTime);
				if (this->CurrentXAccel < 0)
					this->CurrentXAccel = 0;
			}
			else
			{
				this->CurrentXAccel += (this->MaxAccel  * delta / this->AccelTime);
				if (this->CurrentXAccel > 0)
					this->CurrentXAccel = 0;
			}
		}
	if(!this->isMoveRight && !this->isMoveLeft)
		if (this->CurrentYAccel != 0)
		{
			if (this->CurrentYAccel > 0)
			{
				this->CurrentYAccel -= (this->MaxAccel * delta / this->AccelTime);
				if (this->CurrentYAccel < 0)
					this->CurrentYAccel = 0;
			}
			else
			{
				this->CurrentYAccel += (this->MaxAccel * delta / this->AccelTime);
				if (this->CurrentYAccel > 0)
					this->CurrentYAccel = 0;
			}
		}

	if (!this->isMoveUp && !this->isMoveDown)
		if (this->CurrentZAccel != 0)
		{
			if (this->CurrentZAccel > 0)
			{
				this->CurrentZAccel -= (this->MaxAccel * delta / this->AccelTime);
				if (this->CurrentZAccel < 0)
					this->CurrentZAccel = 0;
			}
			else
			{
				this->CurrentZAccel += (this->MaxAccel * delta / this->AccelTime);
				if (this->CurrentZAccel > 0)
					this->CurrentZAccel = 0;
			}
		}

	this->ResultVector = diffX * this->CurrentXAccel;
	this->ResultVector += diffY * this->CurrentYAccel;
	this->ResultVector += diffZ * this->CurrentZAccel;

	this->CurrentSpeed = glm::sqrt(glm::pow(this->ResultVector.x, 2) + glm::pow(this->ResultVector.y, 2) + glm::pow(this->ResultVector.y, 2));

	if (this->CurrentSpeed > this->MaxSpeed)
	{

	}

	this->_ParrentObject->SetPosition(this->_ParrentObject->GetPosition()+ResultVector);
	//CLog::MyLog(0, "Movement %f, %f, %f - Accels: %f,%f - Speed: %f - Tick: %d", this->ResultVector.x, this->ResultVector.y,this->ResultVector.z,this->CurrentXAccel,this->CurrentYAccel,this->CurrentSpeed,delta);
}
