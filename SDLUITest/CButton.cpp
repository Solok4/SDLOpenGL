#include "pch.h"
#include "CButton.h"
#include <SDL.h>
#include "CLog.h"


CButton::CButton(unsigned int ID)
{
	this->_ID = ID;
	this->_Size = vec2(20.0f, 10.0f);
	this->_Position = vec3(0.0f);
	this->start = SDL_GetTicks();
}


CButton::~CButton()
{
}

void CButton::IsClicked(SDL_MouseButtonEvent MouseData)
{

	if (this->_Position.x<MouseData.x &&this->_Position.x + this->_Size.x>MouseData.x && this->_Position.y<MouseData.y && this->_Position.y + this->_Size.y>MouseData.y)
	{
		current = SDL_GetTicks();
		uint32 Difference = current - start;
		if(Difference >200)
		{
			this->OnClick();
			start = SDL_GetTicks();
		}
	}
}

void CButton::AttachFunc(void(*fun)())
{
	this->Function = fun;
}

void CButton::OnClick()
{
	this->Function();
}


void CButton::Prepare()
{
	float vertices[] = {
		0.0f,				0.0f,				0.0f,
		(this->_Size.x ),	0.0f,				0.0f,
		(this->_Size.x ),	(this->_Size.y ),	0.0f,

		(this->_Size.x ),	(this->_Size.y),	0.0f,
		0.0f,				(this->_Size.y ),	0.0f,
		0.0f,				0.0f,				0.0f

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
