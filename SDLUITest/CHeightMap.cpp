#include "CHeightMap.h"
#include "COpengl.h"

CHeightMap::~CHeightMap()
{
}

void CHeightMap::CreateMesh()
{
	std::vector<glm::vec3> MapBuffer;
	std::vector<glm::vec2> UVBuffer;
	std::vector<glm::vec2> NormalBuffer;
	int counter = 0;
	for(int CurrentHeight = 0;CurrentHeight<this->_Size-1;CurrentHeight++)
		for (int CurrentWidth = 0; CurrentWidth < this->_Size-1 ; CurrentWidth++)
		{
			//First triangle
			//1
			MapBuffer.push_back(glm::vec3((-(this->_Size / 2) + CurrentWidth+1) * this->_Distance,
				this->_HeightMap[CurrentHeight][CurrentWidth+1],
				(-(this->_Size / 2) + CurrentHeight) * this->_Distance));
			//2
			MapBuffer.push_back(glm::vec3((-(this->_Size / 2) + CurrentWidth) * this->_Distance,
				this->_HeightMap[CurrentHeight][CurrentWidth],
				(-(this->_Size / 2) + CurrentHeight) * this->_Distance));
			//3
			MapBuffer.push_back(glm::vec3((-(this->_Size / 2) + CurrentWidth) * this->_Distance,
				this->_HeightMap[CurrentHeight + 1][CurrentWidth],
				(-(this->_Size / 2) + CurrentHeight+1) * this->_Distance));

			//Second triangle
			//3
			MapBuffer.push_back(glm::vec3((-(this->_Size / 2) + CurrentWidth) * this->_Distance,
				this->_HeightMap[CurrentHeight+1][CurrentWidth],
				(-(this->_Size / 2) + CurrentHeight+1) * this->_Distance));
			//4
			MapBuffer.push_back(glm::vec3((-(this->_Size / 2) + CurrentWidth+1) * this->_Distance,
				this->_HeightMap[CurrentHeight + 1][CurrentWidth+1],
				(-(this->_Size / 2) + CurrentHeight + 1) * this->_Distance));
			//1
			MapBuffer.push_back(glm::vec3((-(this->_Size / 2) + CurrentWidth+1) * this->_Distance,
				this->_HeightMap[CurrentHeight][CurrentWidth+1],
				(-(this->_Size / 2) + CurrentHeight) * this->_Distance));
			this->_VertexCount += 6;
		}

	for (int CurrentHeight = 0; CurrentHeight < this->_Size - 1; CurrentHeight++)
		for (int CurrentWidth = 0; CurrentWidth < this->_Size - 1; CurrentWidth++)
		{
			//First triangle
			UVBuffer.push_back(glm::vec2(0.0f));
			UVBuffer.push_back(glm::vec2(1.0f,0.0f));
			UVBuffer.push_back(glm::vec2(0.f,1.f));

			//Second triangle
			UVBuffer.push_back(glm::vec2(1.0f,0.f));
			UVBuffer.push_back(glm::vec2(1.0f, 1.0f));
			UVBuffer.push_back(glm::vec2(0.f, 1.f));
		}

	glGenVertexArrays(1, &this->_HeightMapVAO);
	glBindVertexArray(this->_HeightMapVAO);
	GLuint VBO[2];
	glGenBuffers(2, VBO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, MapBuffer.size()*sizeof(glm::vec3), &MapBuffer[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(0);

	//glEnableVertexAttribArray(MODEL_TEXCORDBUFFER);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//glBufferData(GL_ARRAY_BUFFER, UVBuffer.size() * sizeof(glm::vec2), &UVBuffer[0], GL_STATIC_DRAW);
	//glVertexAttribPointer(MODEL_TEXCORDBUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//glDisableVertexAttribArray(MODEL_TEXCORDBUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CHeightMap::Draw()
{
	glBindVertexArray(this->_HeightMapVAO);
	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(MODEL_TEXCORDBUFFER);
	glDrawArrays(GL_TRIANGLES, 0, this->_VertexCount);
	//glDisableVertexAttribArray(MODEL_TEXCORDBUFFER);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

CHeightMap::CHeightMap(int size, float distance, int seed)
{
	this->_Distance = distance;
	this->_Seed = seed;
	this->_VertexCount = 0;
	this->_Position = glm::vec3(0.0f);
	this->_Rotation = glm::vec3(0.f,0.f,0.0f);
	this->_Scale = glm::vec3(1.0f);
	if (size >= HEIGHT_MAP_MAX_SIZE)
	{
		this->_Size = HEIGHT_MAP_MAX_SIZE;
	}
	else
	{
		this->_Size = size;
	}

	this->_HeightMap.resize(this->_Size, std::vector<float>(this->_Size, 0.0f));
}

void CHeightMap::SetPosition(glm::vec3 pos)
{
	this->_Position = pos;
	this->CalculateMatrix();
}

glm::vec3 CHeightMap::GetPosition()
{
	return this->_Position;
}

void CHeightMap::SetRotation(glm::vec3 rot)
{

	this->_Rotation = rot;
	this->CalculateMatrix();
}

glm::vec3 CHeightMap::GetRotation()
{
	return this->_Rotation;
}

void CHeightMap::SetScale(glm::vec3 scale)
{
	this->_Scale = scale;
	this->CalculateMatrix();
}


glm::vec3 CHeightMap::GetScale()
{
	return this->_Scale;
}

void CHeightMap::CalculateMatrix()
{
	glm::mat4 Translation = glm::translate(glm::mat4(), glm::vec3(this->_Position.x, this->_Position.y, this->_Position.z));
	glm::mat4 Scaling = glm::scale(glm::vec3(this->_Scale));
	glm::mat4 RotationX = glm::rotate(glm::radians(this->_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 RotationY = glm::rotate(glm::radians(this->_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 RotationZ = glm::rotate(glm::radians(this->_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 Rotation = RotationX * RotationY * RotationZ;
	this->_ModelMatrix = Translation * Rotation * Scaling;
}

glm::mat4 CHeightMap::GetModelMatrix()
{
	return this->_ModelMatrix;
}
