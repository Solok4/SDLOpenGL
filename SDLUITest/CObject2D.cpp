#include "pch.h"
#include "CObject2D.h"
#include "glm.hpp"
#include "gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"
#include <fstream>
#include "SDL_image.h"
#include "CLog.h"


CObject2D::CObject2D()
{
	this->ModelMatrix = glm::mat4(1.0f);
	this->_VAO = 0;
	this->_VBO.clear();
	this->Layer = 0;
	this->SetPosition(glm::vec2(0.0f,0.0f));
	this->SetSize(glm::vec2(1.0f, 1.0f));
	this->SetRotation(glm::vec3(0.f, 0.f, 0.f));
}


CObject2D::~CObject2D()
{
	this->ModelMatrix = glm::mat4(1.0f);
	this->_VAO = 0;
	this->_VBO.clear();
	this->Layer = 0;
}



void CObject2D::Prepare()
{
	float vertices[] = {
		-0.5f,	0.5f,	0.0f,
		0.5f,	0.5f,	0.0f,
		0.5f,	-0.5f,	0.0f,
		-0.5f,	-0.5f,	0.0f,
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


	this->VertexCount = sizeof(vertices) / 3;
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



}

void CObject2D::RefreshModelMatrix()
{
	mat4 Translation = translate(mat4(), vec3(this->_Position.x, this->_Position.y, (float)Layer));
	mat4 Scaling = scale(vec3(this->_Size.x, this->_Size.y, 1.0f));
	mat4 RotationX = rotate(radians(this->_Rotation.x), vec3(1.0f, 0.0f, 0.0f));
	mat4 RotationY = rotate(radians(this->_Rotation.y), vec3(0.0f, 1.0f, 0.0f));
	mat4 RotationZ = rotate(radians(this->_Rotation.z), vec3(0.0f, 0.0f, 1.0f));
	mat4 Rotation = RotationX * RotationY *RotationZ;
	this->ModelMatrix = Translation * Rotation* Scaling;

}

void CObject2D::SetPosition(vec2 vec)
{
	this->_Position = vec;
	this->RefreshModelMatrix();
}

void CObject2D::SetRotation(vec3 vec)
{
	this->_Rotation = vec;
	this->RefreshModelMatrix();

}

void CObject2D::SetSize(vec2 vec)
{
	this->_Size = vec;
	this->RefreshModelMatrix();
}

void CObject2D::Draw()
{
	glBindVertexArray(this->_VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->TextureID);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void CObject2D::MoveObjectLayerDown()
{
	if (this->Layer <= 0)
	{
		this->Layer--;

	}
}

void CObject2D::MoveObjectLayerUp()
{
	if (this->Layer >= -255)
	{
		this->Layer++;
	}
}

void CObject2D::SetObjectLayer(int Layer)
{
	if (this->Layer >= -255 && this->Layer <= 0)
	{
		this->Layer = -Layer;
	}
}

int CObject2D::GetObjectLayer()
{
	return this->Layer;
}

void CObject2D::SetTexture(const char * str)
{
	std::string FileName(str);
	int lastSlash = FileName.find_last_of("/");
	std::string JustFile(FileName.substr(lastSlash + 1));
	/*int dot = JustFile.find_last_of(".");
	std::string Extension(JustFile.substr(dot + 1));*/

	SDL_Surface* Tex;
	Tex = IMG_Load(str);
	if (!Tex)
	{
		CLog::MyLog(1, "Failed to load a texture: " + FileName + " " + IMG_GetError());
	}

	glGenTextures(1, &this->TextureID);
	glBindTexture(GL_TEXTURE_2D, this->TextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Tex->w, Tex->h, 0, GL_RGB, GL_UNSIGNED_BYTE, Tex->pixels);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

}



mat4 CObject2D::GetModelMatrix()
{
	return this->ModelMatrix;
}
