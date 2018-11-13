#include "pch.h"
#include "CObject3D.h"
#include "gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"

CObject3D::CObject3D(unsigned int ID)
{
	this->_ID = ID;
	this->CameraDistance = 10;
}


CObject3D::~CObject3D()
{
}

void CObject3D::Prepare()
{
}

void CObject3D::PreDraw()
{
	mat4 Translation = translate(mat4(), vec3(this->_Position.x, this->_Position.y, 0.0f));
	mat4 Scaling = scale(vec3(1.0f, 1.0f, 1.0f));
	mat4 RotationX = rotate(radians(this->_Rotation.x), vec3(1.0f, 0.0f, 0.0f));
	mat4 RotationY = rotate(radians(this->_Rotation.y), vec3(0.0f, 1.0f, 0.0f));
	mat4 RotationZ = rotate(radians(this->_Rotation.z), vec3(0.0f, 0.0f, 1.0f));
	mat4 Rotation = RotationX * RotationY *RotationZ;
	this->ModelMatrix = Translation * Rotation* Scaling;
}

void CObject3D::SetPosition(vec3 vec)
{
	this->_Position.x = vec.x;
	this->_Position.y = vec.y;
	this->_Position.z = vec.z;
}

void CObject3D::SetRotation(vec3 vec)
{
	this->_Rotation.x = vec.x;
	this->_Rotation.y = vec.y;
	this->_Rotation.z = vec.z;
}

void CObject3D::SetScale(vec3 vec)
{
	this->_Scale.x = vec.x;
	this->_Scale.y = vec.y;
	this->_Scale.z = vec.z;
}

void CObject3D::Draw()
{
}

void CObject3D::Free()
{
}
