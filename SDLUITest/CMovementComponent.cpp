#include "pch.h"
#include "CMovementComponent.h"


CMovementComponent::CMovementComponent()
{
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


void CMovementComponent::Tick(uint32_t delta)
{
	CBaseComponent::Tick(delta);
	glm::vec3 diffX = this->GetForwardVector() - this->_ParrentObject->GetPosition();
	glm::vec3 diffY = this->GetRightVector() - this->_ParrentObject->GetPosition();
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
		if (this->CurrentXAccel < this->MaxAccel)
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
		if (this->CurrentYAccel < this->MaxAccel)
			this->CurrentYAccel -= (this->MaxAccel  * delta / this->AccelTime);
		
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
				this->CurrentYAccel -= (this->MaxAccel / this->AccelTime)*delta;
				if (this->CurrentYAccel < 0)
					this->CurrentYAccel = 0;
			}
			else
			{
				this->CurrentYAccel += (this->MaxAccel / this->AccelTime)*delta;
				if (this->CurrentYAccel > 0)
					this->CurrentYAccel = 0;
			}
		}

	this->ResultVector = diffX * this->CurrentXAccel;
	this->ResultVector += diffY * this->CurrentYAccel;

	this->CurrentSpeed = glm::sqrt(glm::pow(this->ResultVector.x, 2) + glm::pow(this->ResultVector.y, 2) + glm::pow(this->ResultVector.y, 2));

	if (this->CurrentSpeed > this->MaxSpeed)
	{

	}

	this->_ParrentObject->SetPosition(this->_ParrentObject->GetPosition()+ResultVector);
	CLog::MyLog(0, "Movement %f, %f, %f - Accels: %f,%f - Speed: %f - Tick: %d", this->ResultVector.x, this->ResultVector.y,this->ResultVector.z,this->CurrentXAccel,this->CurrentYAccel,this->CurrentSpeed,delta);
}
