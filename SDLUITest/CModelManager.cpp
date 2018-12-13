#include "pch.h"
#include "CModelManager.h"
#include <fstream>
#include "CLog.h"
#include "glm.hpp"


CModelManager::CModelManager()
{
}


CModelManager::~CModelManager()
{
}

int CModelManager::LoadOBJ(const char * path)
{
	FILE* file;
	fopen_s(&file,path, "r");
	if (file == NULL)
	{
		CLog::MyLog(1, "Failed to load a model from " + std::string(path));
		return 1;
	}
	std::shared_ptr<Model> tempModel (new Model);
	std::vector<glm::vec3> temp_Vertices;
	std::vector<glm::vec2> temp_Texcords;
	std::vector<glm::vec3> temp_Normals;
	std::vector<unsigned int> VertexIndices, TexcordIndices, NormalIndices;

	std::vector<glm::vec3> out_Vertices;
	std::vector<glm::vec2> out_Texcords;
	std::vector<glm::vec3> out_Normals;

	while (1)
	{
		char LineHeader[128];
		int res = fscanf_s(file, "%s", LineHeader,sizeof(LineHeader));
		if (res == EOF)
			break;

		if (strcmp(LineHeader, "v") == 0)
		{
			glm::vec3 Vertex;
			fscanf_s(file, "%f %f %f", &Vertex.x, &Vertex.y, &Vertex.z);
			temp_Vertices.push_back(Vertex);
		}
		else if (strcmp(LineHeader, "vt") == 0)
		{
			tempModel->HasTexcords = true;
			glm::vec2 Texcord;
			fscanf_s(file, "%f %f", &Texcord.x, &Texcord.y);
			temp_Texcords.push_back(Texcord);
		}
		else if (strcmp(LineHeader, "vn") == 0)
		{
			tempModel->HasNormals = true;
			glm::vec3 Normal;
			fscanf_s(file, "%f %f %f", &Normal.x, &Normal.y, &Normal.z);
			temp_Normals.push_back(Normal);
		}
		else if (strcmp(LineHeader, "f") == 0)
		{
			if (tempModel->HasNormals && tempModel->HasTexcords)
			{
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}
				VertexIndices.push_back(vertexIndex[0]);
				VertexIndices.push_back(vertexIndex[1]);
				VertexIndices.push_back(vertexIndex[2]);
				TexcordIndices.push_back(uvIndex[0]);
				TexcordIndices.push_back(uvIndex[1]);
				TexcordIndices.push_back(uvIndex[2]);
				NormalIndices.push_back(normalIndex[0]);
				NormalIndices.push_back(normalIndex[1]);
				NormalIndices.push_back(normalIndex[2]);
			}
			else if (tempModel->HasNormals && !tempModel->HasTexcords)
			{
				unsigned int vertexIndex[3], normalIndex[3];
				int matches = fscanf_s(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
				if (matches != 6) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}
				VertexIndices.push_back(vertexIndex[0]);
				VertexIndices.push_back(vertexIndex[1]);
				VertexIndices.push_back(vertexIndex[2]);
				NormalIndices.push_back(normalIndex[0]);
				NormalIndices.push_back(normalIndex[1]);
				NormalIndices.push_back(normalIndex[2]);
			}
			else if (!tempModel->HasNormals && tempModel->HasTexcords)
			{
				unsigned int vertexIndex[3], uvIndex[3];
				int matches = fscanf_s(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0],&vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
				if (matches != 6) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}
				VertexIndices.push_back(vertexIndex[0]);
				VertexIndices.push_back(vertexIndex[1]);
				VertexIndices.push_back(vertexIndex[2]);
				TexcordIndices.push_back(uvIndex[0]);
				TexcordIndices.push_back(uvIndex[1]);
				TexcordIndices.push_back(uvIndex[2]);
			}


		}
	}
	for (unsigned int i = 0; i < VertexIndices.size(); i++)
	{
		unsigned int VertIndex = VertexIndices[i];
		glm::vec3 vertex = temp_Vertices[VertIndex - 1];
		out_Vertices.push_back(vertex);
	}
	if (tempModel->HasTexcords)
	{
		for (unsigned int i = 0; i < TexcordIndices.size(); i++)
		{
			unsigned int TexcordIndex = TexcordIndices[i];
			glm::vec2 texcord = temp_Texcords[TexcordIndex - 1];
			out_Texcords.push_back(texcord);
		}
	}
	if (tempModel->HasNormals)
	{
		for (unsigned int i = 0; i < NormalIndices.size(); i++)
		{
			unsigned int NormalIndex = NormalIndices[i];
			glm::vec3 normal = temp_Normals[NormalIndex - 1];
			out_Normals.push_back(normal);
		}
	}
	tempModel->IndicesCount = VertexIndices.size();

	glGenVertexArrays(1, &tempModel->VAO);
	glBindVertexArray(tempModel->VAO);
	glGenBuffers(3, tempModel->VBOs);
	glGenBuffers(1, &tempModel->EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempModel->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, VertexIndices.size(), VertexIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, tempModel->VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, out_Vertices.size(), out_Vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, tempModel->VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, out_Texcords.size(), out_Texcords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, tempModel->VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, out_Normals.size(), out_Normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	std::string name(path);
	int slash = name.find_last_of("/");
	this->Models.emplace(name.substr(slash+1, slash + 2), tempModel);
	return 0;
}

std::shared_ptr<Model> CModelManager::GetModelByName(std::string name)
{
	for (std::map<std::string, std::shared_ptr<Model>>::iterator it = this->Models.begin(); it != this->Models.end(); ++it)
	{
		if (it->first == name)
		{
			return it->second;
		}
	}
	return {};
}
