#include "CModelManager.h"
#include <fstream>
#include "../../CLog.h"
#include "../../glm/glm.hpp"
#include "../shared/Shared.h"

std::unique_ptr<CModelManager> ModelManager;

CModelManager::CModelManager()
{
}

CModelManager::~CModelManager()
{
	CLog::debug("ModelManagerDestructor");
}

void CModelManager::LoadOBJ(const char* path)
{
	CLog::info("File open %s", path);
	FILE *file = StdLibWrapper::Sfopen(path, "r");
	if (file == NULL)
	{
		CLog::error("Failed to load a model from %s", path);
		return;
	}
	const char* name(path);
	const char* slash = strrchr(name, '/');
	const char* JustName(slash + 1);

	CLog::debug("Loading model %s", JustName);

	std::shared_ptr<Model> tempModel(new Model);
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
		int res = StdLibWrapper::Sfscanf(file,  sizeof(LineHeader) / sizeof(LineHeader[0]), "%s", LineHeader);

		if (res == EOF)
			break;

		if (strcmp(LineHeader, "v") == 0)
		{
			glm::vec3 Vertex;
			StdLibWrapper::Sfscanf(file,  sizeof(glm::vec3), "%f %f %f", &Vertex.x, &Vertex.y, &Vertex.z);

			temp_Vertices.push_back(Vertex);
		}
		else if (strcmp(LineHeader, "vt") == 0)
		{
			tempModel->HasTexcords = true;
			glm::vec2 Texcord;
			StdLibWrapper::Sfscanf(file,  sizeof(glm::vec2), "%f %f", &Texcord.x, &Texcord.y);

			Texcord.y = 1.f - Texcord.y;
			temp_Texcords.push_back(Texcord);
		}
		else if (strcmp(LineHeader, "vn") == 0)
		{
			tempModel->HasNormals = true;
			glm::vec3 Normal;
			StdLibWrapper::Sfscanf(file,  sizeof(glm::vec3), "%f %f %f", &Normal.x, &Normal.y, &Normal.z);

			temp_Normals.push_back(Normal);
		}
		else if (strcmp(LineHeader, "f") == 0)
		{
			if (tempModel->HasNormals && tempModel->HasTexcords)
			{
				unsigned int vertexIndex[5], uvIndex[5], normalIndex[5];
				int matches = StdLibWrapper::Sfscanf(file,
					sizeof(unsigned int)*3 *5,
					"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2],
					&vertexIndex[3], &uvIndex[3], &normalIndex[3],
					&vertexIndex[4], &uvIndex[4], &normalIndex[4]);
				if (matches == 9) {
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
				else if (matches == 12)
				{
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[1]);
					VertexIndices.push_back(vertexIndex[2]);
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[2]);
					VertexIndices.push_back(vertexIndex[3]);
					TexcordIndices.push_back(uvIndex[0]);
					TexcordIndices.push_back(uvIndex[1]);
					TexcordIndices.push_back(uvIndex[2]);
					TexcordIndices.push_back(uvIndex[0]);
					TexcordIndices.push_back(uvIndex[2]);
					TexcordIndices.push_back(uvIndex[3]);
					NormalIndices.push_back(normalIndex[0]);
					NormalIndices.push_back(normalIndex[1]);
					NormalIndices.push_back(normalIndex[2]);
					NormalIndices.push_back(normalIndex[0]);
					NormalIndices.push_back(normalIndex[2]);
					NormalIndices.push_back(normalIndex[3]);
				}
				else if (matches == 15)
				{
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[1]);
					VertexIndices.push_back(vertexIndex[2]);
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[2]);
					VertexIndices.push_back(vertexIndex[3]);
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[3]);
					VertexIndices.push_back(vertexIndex[4]);
					TexcordIndices.push_back(uvIndex[0]);
					TexcordIndices.push_back(uvIndex[1]);
					TexcordIndices.push_back(uvIndex[2]);
					TexcordIndices.push_back(uvIndex[0]);
					TexcordIndices.push_back(uvIndex[2]);
					TexcordIndices.push_back(uvIndex[3]);
					TexcordIndices.push_back(uvIndex[0]);
					TexcordIndices.push_back(uvIndex[3]);
					TexcordIndices.push_back(uvIndex[4]);
					NormalIndices.push_back(normalIndex[0]);
					NormalIndices.push_back(normalIndex[1]);
					NormalIndices.push_back(normalIndex[2]);
					NormalIndices.push_back(normalIndex[0]);
					NormalIndices.push_back(normalIndex[2]);
					NormalIndices.push_back(normalIndex[3]);
					NormalIndices.push_back(normalIndex[0]);
					NormalIndices.push_back(normalIndex[3]);
					NormalIndices.push_back(normalIndex[4]);
				}
				else
				{
					if (matches == -1)	break;
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					fclose(file);
					return;
				}
			}
			else if (tempModel->HasNormals && !tempModel->HasTexcords)
			{
				unsigned int vertexIndex[5], normalIndex[5];
				int matches = StdLibWrapper::Sfscanf(file, 
					sizeof(unsigned int)*2 *5,
					"%d//%d %d//%d %d//%d %d//%d %d//%d\n",
					&vertexIndex[0], &normalIndex[0],
					&vertexIndex[1], &normalIndex[1],
					&vertexIndex[2], &normalIndex[2],
					&vertexIndex[3], &normalIndex[3],
					&vertexIndex[4], &normalIndex[4]);

				if (matches == 6) {
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[1]);
					VertexIndices.push_back(vertexIndex[2]);
					NormalIndices.push_back(normalIndex[0]);
					NormalIndices.push_back(normalIndex[1]);
					NormalIndices.push_back(normalIndex[2]);
				}
				else if (matches == 8)
				{
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[1]);
					VertexIndices.push_back(vertexIndex[2]);
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[2]);
					VertexIndices.push_back(vertexIndex[3]);
					NormalIndices.push_back(normalIndex[0]);
					NormalIndices.push_back(normalIndex[1]);
					NormalIndices.push_back(normalIndex[2]);
					NormalIndices.push_back(normalIndex[0]);
					NormalIndices.push_back(normalIndex[2]);
					NormalIndices.push_back(normalIndex[3]);
				}
				else if (matches == 10)
				{
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[1]);
					VertexIndices.push_back(vertexIndex[2]);
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[2]);
					VertexIndices.push_back(vertexIndex[3]);
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[3]);
					VertexIndices.push_back(vertexIndex[4]);
					NormalIndices.push_back(normalIndex[0]);
					NormalIndices.push_back(normalIndex[1]);
					NormalIndices.push_back(normalIndex[2]);
					NormalIndices.push_back(normalIndex[0]);
					NormalIndices.push_back(normalIndex[2]);
					NormalIndices.push_back(normalIndex[3]);
					NormalIndices.push_back(normalIndex[0]);
					NormalIndices.push_back(normalIndex[3]);
					NormalIndices.push_back(normalIndex[4]);
				}
				else
				{
					if (matches == -1)	break;
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					fclose(file);
					return;
				}
			}
			else if (!tempModel->HasNormals && tempModel->HasTexcords)
			{
				unsigned int vertexIndex[5], uvIndex[5];
				
				int matches = StdLibWrapper::Sfscanf(file,
					sizeof(unsigned int)*2 *5,
					"%d/%d %d/%d %d/%d %d/%d %d/%d\n",
					&vertexIndex[0], &uvIndex[0],
					&vertexIndex[1], &uvIndex[1],
					&vertexIndex[2], &uvIndex[2],
					&vertexIndex[3], &uvIndex[3],
					&vertexIndex[4], &uvIndex[4]);
				if (matches == 6) {
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[1]);
					VertexIndices.push_back(vertexIndex[2]);
					TexcordIndices.push_back(uvIndex[0]);
					TexcordIndices.push_back(uvIndex[1]);
					TexcordIndices.push_back(uvIndex[2]);
				}
				else if (matches == 8)
				{
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[1]);
					VertexIndices.push_back(vertexIndex[2]);
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[2]);
					VertexIndices.push_back(vertexIndex[3]);
					TexcordIndices.push_back(uvIndex[0]);
					TexcordIndices.push_back(uvIndex[1]);
					TexcordIndices.push_back(uvIndex[2]);
					TexcordIndices.push_back(uvIndex[0]);
					TexcordIndices.push_back(uvIndex[2]);
					TexcordIndices.push_back(uvIndex[3]);
				}
				else if (matches == 10)
				{
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[1]);
					VertexIndices.push_back(vertexIndex[2]);
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[2]);
					VertexIndices.push_back(vertexIndex[3]);
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[3]);
					VertexIndices.push_back(vertexIndex[4]);
					TexcordIndices.push_back(uvIndex[0]);
					TexcordIndices.push_back(uvIndex[1]);
					TexcordIndices.push_back(uvIndex[2]);
					TexcordIndices.push_back(uvIndex[0]);
					TexcordIndices.push_back(uvIndex[2]);
					TexcordIndices.push_back(uvIndex[3]);
					TexcordIndices.push_back(uvIndex[0]);
					TexcordIndices.push_back(uvIndex[3]);
					TexcordIndices.push_back(uvIndex[4]);
				}
				else
				{
					if (matches == -1)	break;
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					fclose(file);
					return;
				}
			}
		}
	}

	fclose(file);

	for (unsigned int i = 0; i < VertexIndices.size(); i++)
	{
		unsigned int VertIndex = VertexIndices[i];
		glm::vec3 vertex = temp_Vertices[VertIndex - 1];
		out_Vertices.push_back(vertex);
		if (tempModel->HasTexcords)
		{
			unsigned int TexcordIndex = TexcordIndices[i];
			glm::vec2 texcord = temp_Texcords[TexcordIndex - 1];
			out_Texcords.push_back(texcord);
		}
		if (tempModel->HasNormals)
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

	glEnableVertexAttribArray(MODEL_MESHBUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, tempModel->VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, out_Vertices.size() * sizeof(glm::vec3), &out_Vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MODEL_MESHBUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (tempModel->HasTexcords)
	{
		glEnableVertexAttribArray(MODEL_TEXCORDBUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, tempModel->VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, out_Texcords.size() * sizeof(glm::vec2), &out_Texcords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(MODEL_TEXCORDBUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glDisableVertexAttribArray(MODEL_TEXCORDBUFFER);
	}

	if (tempModel->HasNormals)
	{
		glEnableVertexAttribArray(MODEL_NORMALBUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, tempModel->VBOs[2]);
		glBufferData(GL_ARRAY_BUFFER, out_Normals.size() * sizeof(glm::vec3), &out_Normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(MODEL_NORMALBUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glDisableVertexAttribArray(MODEL_NORMALBUFFER);
	}
	glDisableVertexAttribArray(MODEL_MESHBUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CLog::debug("Loaded model: %s", path);
	CLog::debug("V: %d, T: %d, N: %d, I: %d", temp_Vertices.size(), temp_Texcords.size(), temp_Normals.size(), VertexIndices.size());

	tempModel->Name = JustName;
	this->Models.push_back(tempModel);
}

std::shared_ptr<Model> CModelManager::GetModelByName(std::string name)
{
	for (auto o : this->Models)
	{
		if (StdLibWrapper::Sstrcmp(o->Name, name.c_str()) == 0)
		{
			return o;
		}
	}
	return {};
}