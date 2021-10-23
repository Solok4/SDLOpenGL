#include "CMovementComponent.h"
#include "CObject3D.h"

CMovementComponent::CMovementComponent(CObject3D* ref) :CBaseComponent(ref)
{
	this->_Type = Object3DComponent::MOVEMENT_COMPONENT;
	this->MaxAccel = 0.1f;
	this->MaxSpeed = 1.0f;
	this->CurrentXAccel = 0;
	this->CurrentYAccel = 0;
	this->CurrentZAccel = 0;
	this->CurrentSpeed = 0;
	this->AccelTime = 200;
	this->SlowDownFactor = 1.2;
	this->isMoveForward = 0.0f;
	this->isMoveRight = 0.0f;
	this->isMoveUp = 0.0f;
}

CMovementComponent::CMovementComponent(const CMovementComponent& move, CObject3D* ref) :CBaseComponent(move, ref)
{
	this->MaxAccel = move.MaxAccel;
	this->MaxSpeed = move.MaxSpeed;
	this->CurrentXAccel = 0;
	this->CurrentYAccel = 0;
	this->CurrentZAccel = 0;
	this->AccelTime = move.AccelTime;
	this->isMoveForward = 0.0f;
	this->isMoveRight = 0.0f;
	this->isMoveUp = 0.0f;
	this->ResultVector = glm::vec3(0.f);
	this->_Type = Object3DComponent::MOVEMENT_COMPONENT;
	this->AttachParrentObject(this->PossesingObject->GetRootComponent());
}

CMovementComponent::~CMovementComponent()
{
}

void CMovementComponent::MoveForward(float isForward)
{
	this->isMoveForward = isForward;
}

void CMovementComponent::MoveHorizontal(float isRight)
{
	this->isMoveRight = isRight;
}

void CMovementComponent::MoveVertical(float isUp)
{
	this->isMoveUp = isUp;
}

void CMovementComponent::Tick(double delta)
{
	CBaseComponent::Tick(delta);
	glm::vec3 diffX = this->GetForwardVector();
	glm::vec3 diffY = this->GetUpVector();
	glm::vec3 diffZ = this->GetRightVector();
	//CLog::MyLog(0, "FVector x:%f y:%f z:%f", this->GetForwardVector().x, this->GetForwardVector().y, this->GetForwardVector().z);
	//CLog::MyLog(LogType::Debug, "Inputs x:%f y:%f z:%f", this->isMoveForward, this->isMoveRight, this->isMoveUp);
	if (this->isMoveForward != 0.0f)
	{
		if (this->CurrentXAccel < this->MaxAccel)
		{
			if ((this->CurrentXAccel < 0 && this->isMoveForward >0.0f) || (this->CurrentXAccel > 0 && this->isMoveForward < 0.0f))
				this->CurrentXAccel = 0;
			if (abs(this->CurrentXAccel) < this->MaxAccel)
				this->CurrentXAccel += (this->MaxAccel * delta / this->AccelTime) * this->isMoveForward;
		}
	}
	if (this->isMoveRight != 0.0f)
	{
		if ((this->CurrentZAccel < 0 && this->isMoveRight >0.0f) || (this->CurrentZAccel > 0 && this->isMoveRight < 0.0f))
			this->CurrentZAccel = 0;
		if (abs(this->CurrentZAccel) < this->MaxAccel)
			this->CurrentZAccel += (this->MaxAccel * delta / this->AccelTime) * this->isMoveRight;
	}
	if (this->isMoveUp != 0.0f)
	{
		if ((this->CurrentYAccel < 0 && this->isMoveUp >0.0f) || (this->CurrentYAccel > 0 && this->isMoveUp < 0.0f))
			this->CurrentYAccel = 0;
		if (abs(this->CurrentYAccel) < this->MaxAccel)
			this->CurrentYAccel += (this->MaxAccel * delta / this->AccelTime) * this->isMoveUp;
	}

	//SLOWDOWN
	if (this->isMoveForward == 0.f)
		if (this->CurrentXAccel != 0)
		{
			if (this->CurrentXAccel > 0)
			{
				this->CurrentXAccel -= (this->MaxAccel * delta / this->AccelTime) * this->SlowDownFactor;
				if (this->CurrentXAccel < 0)
					this->CurrentXAccel = 0;
			}
			else
			{
				this->CurrentXAccel += (this->MaxAccel * delta / this->AccelTime) * this->SlowDownFactor;
				if (this->CurrentXAccel > 0)
					this->CurrentXAccel = 0;
			}
		}
	if (this->isMoveRight == 0.f)
		if (this->CurrentZAccel != 0)
		{
			if (this->CurrentZAccel > 0)
			{
				this->CurrentZAccel -= (this->MaxAccel * delta / this->AccelTime) * this->SlowDownFactor;
				if (this->CurrentZAccel < 0)
					this->CurrentZAccel = 0;
			}
			else
			{
				this->CurrentZAccel += (this->MaxAccel * delta / this->AccelTime) * this->SlowDownFactor;
				if (this->CurrentZAccel > 0)
					this->CurrentZAccel = 0;
			}
		}

	if (this->isMoveUp == 0.f)
		if (this->CurrentYAccel != 0)
		{
			if (this->CurrentYAccel > 0)
			{
				this->CurrentYAccel -= (this->MaxAccel * delta / this->AccelTime) * this->SlowDownFactor;
				if (this->CurrentYAccel < 0)
					this->CurrentYAccel = 0;
			}
			else
			{
				this->CurrentYAccel += (this->MaxAccel * delta / this->AccelTime) * this->SlowDownFactor;
				if (this->CurrentYAccel > 0)
					this->CurrentYAccel = 0;
			}
		}

	this->ResultVector = diffX * (float)this->CurrentXAccel;
	this->ResultVector += diffY * (float)this->CurrentYAccel;
	this->ResultVector -= diffZ * (float)this->CurrentZAccel;

	this->CurrentSpeed = glm::sqrt(glm::pow(this->ResultVector.x, 2) + glm::pow(this->ResultVector.y, 2) + glm::pow(this->ResultVector.y, 2));

	if (this->CurrentSpeed > this->MaxSpeed)
	{
	}

	this->_ParrentObject->SetPosition(this->_ParrentObject->GetPosition() + ResultVector);
	//CLog::MyLog(LogType::Debug, "Movement %f, %f, %f - Accels: %f,%f,%f - Speed: %f - Tick: %f", this->ResultVector.x, this->ResultVector.y,this->ResultVector.z,this->CurrentXAccel,this->CurrentYAccel,this->CurrentZAccel,this->CurrentSpeed,delta);
}