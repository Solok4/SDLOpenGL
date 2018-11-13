#include "pch.h"
#include "CObject2D.h"
#include "glm.hpp"
#include "gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"


CObject2D::CObject2D()
{
	this->ModelMatrix = glm::mat4(1.0f);
	this->_VAO = 0;
	this->_VBO.clear();
}


CObject2D::~CObject2D()
{
}



void CObject2D::Prepare()
{
	float vertices[] = {
		-(this->_Size.x / 2),	-(this->_Size.y / 2),	0.0f,
		(this->_Size.x / 2),	-(this->_Size.y / 2),	0.0f,
		(this->_Size.x / 2),	(this->_Size.y / 2),	0.0f,

		(this->_Size.x / 2),	(this->_Size.y / 2),	0.0f,
		-(this->_Size.x / 2),	(this->_Size.y / 2),	0.0f,
		-(this->_Size.x / 2),	-(this->_Size.y / 2),	0.0f

	};

	this->VertexCount = sizeof(vertices) / 3;
	glGenVertexArrays(1, &this->_VAO);
	glBindVertexArray(this->_VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	this->_VBO.push_back(VBO);
}

void CObject2D::PreDraw()
{
	mat4 Translation = translate(mat4(), vec3(this->_Position.x, this->_Position.y, 0.0f));
	mat4 Scaling = scale(vec3(1.0f, 1.0f, 1.0f));
	mat4 RotationX = rotate(radians(this->_Rotation.x), vec3(1.0f, 0.0f, 0.0f));
	mat4 RotationY = rotate(radians(this->_Rotation.y), vec3(0.0f, 1.0f, 0.0f));
	mat4 RotationZ = rotate(radians(this->_Rotation.z), vec3(0.0f, 0.0f, 1.0f));
	mat4 Rotation = RotationX * RotationY *RotationZ;
	this->ModelMatrix = Translation * Rotation* Scaling;

}

void CObject2D::SetPosition(vec2 vec)
{
	this->_Position = vec;
}

void CObject2D::SetRotation(vec3 vec)
{
	this->_Rotation = vec;

}

void CObject2D::SetSize(vec2 vec)
{
	this->_Size = vec;
}

void CObject2D::Draw()
{
	glBindVertexArray(this->_VAO);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, this->VertexCount);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void CObject2D::Free()
{
}


mat4 CObject2D::GetModelMatrix()
{
	return this->ModelMatrix;
}
