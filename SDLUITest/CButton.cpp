#include "pch.h"
#include "CButton.h"
#include <SDL.h>
#include "CLog.h"


CButton::CButton()
{
	this->_Size = vec2(1.0f);
	this->_Position = vec3(0.0f);
	this->_Rotation = vec3(0.f);
	Label = new CLabel();
}


CButton::~CButton()
{
}

void CButton::IsClicked(SDL_MouseButtonEvent MouseData)
{

	if (this->_Position.x<MouseData.x &&this->_Position.x + this->_Size.x>MouseData.x && this->_Position.y<MouseData.y && this->_Position.y + this->_Size.y>MouseData.y)
	{

		this->OnHover();

		if (MouseData.button == SDL_BUTTON_LEFT && MouseData.state == SDL_PRESSED)
		{
			if (!this->Pressed)
			{
				this->Func();
				this->Pressed = true;
			}
		}

	}
	if (MouseData.button == SDL_BUTTON_LEFT && MouseData.state == SDL_RELEASED)
	{
		this->Pressed = false;
	}
}

void CButton::AttachFunc(void(*fun)())
{
	this->Func = fun;
}

void CButton::OnHover()
{
}


void CButton::Prepare()
{
	float vertices[] = {
		0.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
		1.0f,	1.0f,	0.0f,

		0.0f,	1.0f,	0.0f,


	};

	float TexCords[] = {
		0.0f,0.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f,
	};

	int Indices[] = {
		0,1,3,
		1,2,3
	};

	glGenVertexArrays(1, &this->_VAO);
	glBindVertexArray(this->_VAO);

	GLuint VBO[2];
	glGenBuffers(2, VBO);
	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCords), TexCords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	this->_VBO.push_back(VBO[0]);
	this->_VBO.push_back(VBO[1]);

	Label->Prepare();
	Label->SetPosition(vec2(this->_Position.x+(this->_Size.x / 2), this->_Position.y+(this->_Size.y/2)));
	Label->MoveObjectLayerUp();
}

CLabel* CButton::GetLabel()
{
	return this->Label;
}
