#include "CHeightMap.h"
#include "COpengl.h"

CHeightMap::~CHeightMap()
{
}

void CHeightMap::CreateMesh()
{
	std::vector<glm::vec3> MapBuffer;
	std::vector<glm::vec2> UVBuffer;
	std::vector<glm::vec3> NormalBuffer;
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

	for (int CurrentHeight = 0; CurrentHeight < this->_Size - 1; CurrentHeight++)
		for (int CurrentWidth = 0; CurrentWidth < this->_Size - 1; CurrentWidth++)
		{
			//First triangle
			NormalBuffer.push_back(glm::vec3(0.5f, 1.0f, 1.0f));
			NormalBuffer.push_back(glm::vec3(0.5f, 1.0f, 1.0f));
			NormalBuffer.push_back(glm::vec3(0.5f, 1.0f, 1.0f));

			//Second triangle
			NormalBuffer.push_back(glm::vec3(0.5f, 1.0f, 1.0f));
			NormalBuffer.push_back(glm::vec3(0.5f, 1.0f, 1.0f));
			NormalBuffer.push_back(glm::vec3(0.5f, 1.0f, 1.0f));
		}

	glGenVertexArrays(1, &this->_HeightMapVAO);
	glBindVertexArray(this->_HeightMapVAO);
	GLuint VBO[3];
	glGenBuffers(3, VBO);
	glEnableVertexAttribArray(MODEL_MESHBUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, MapBuffer.size()*sizeof(glm::vec3), &MapBuffer[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MODEL_MESHBUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(MODEL_MESHBUFFER);

	glEnableVertexAttribArray(MODEL_TEXCORDBUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, UVBuffer.size() * sizeof(glm::vec2), &UVBuffer[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MODEL_TEXCORDBUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(MODEL_TEXCORDBUFFER);

	glEnableVertexAttribArray(MODEL_NORMALBUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, NormalBuffer.size() * sizeof(glm::vec3), &NormalBuffer[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MODEL_NORMALBUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(MODEL_NORMALBUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CHeightMap::Draw()
{
	auto oglShaderClass = OpenGL->GetShadersClass();
	auto matData = this->_HeightmapMaterial->GetData();
	auto program = OpenGL->GetShadersClass().GetCurrentShaderProgram();

	glBindVertexArray(this->_HeightMapVAO);
	glEnableVertexAttribArray(MODEL_MESHBUFFER);
	glEnableVertexAttribArray(MODEL_TEXCORDBUFFER);
	glEnableVertexAttribArray(MODEL_NORMALBUFFER);

	oglShaderClass.Uniform3f(matData->AmbientColor, "Mat.Ambient");
	oglShaderClass.Uniform3f(matData->DiffuseColor, "Mat.Diffuse");
	oglShaderClass.Uniform3f(matData->SpecularColor, "Mat.Specular");
	oglShaderClass.Uniform1f(matData->Shininess, "Mat.Shininess");
	glActiveTexture(GL_TEXTURE0);
	auto base = matData->DiffuseTex;
	if (!base)
	{
		return;
	}
	glBindTexture(GL_TEXTURE_2D, base);
	glUniform1i(glGetUniformLocation(program, "Base"), 0);
	auto normal = matData->NormalTex;
	if (normal != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal);
		glUniform1i(glGetUniformLocation(program, "Normal"), 1);
	}
	auto specular = matData->SpecularTex;
	if (specular != -1)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, specular);
		glUniform1i(glGetUniformLocation(program, "Specular"), 2);
	}

	glDrawArrays(GL_TRIANGLES, 0, this->_VertexCount);

	glDisableVertexAttribArray(MODEL_NORMALBUFFER);
	glDisableVertexAttribArray(MODEL_TEXCORDBUFFER);
	glDisableVertexAttribArray(MODEL_MESHBUFFER);
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
	this->_HeightmapMaterial = MaterialManager->GetDefaultMaterial();
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
