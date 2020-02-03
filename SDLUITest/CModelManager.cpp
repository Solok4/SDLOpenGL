#include "pch.h"
#include "CModelManager.h"
#include <fstream>
#include "CLog.h"
#include "glm/glm.hpp"

std::unique_ptr<CModelManager> ModelManager;

CModelManager::CModelManager()
{

}


CModelManager::~CModelManager()
{
	CLog::MyLog(LogType::Log, "ModelManagerDestructor");
}


void CModelManager::LoadOBJ(const char * path)
{
	FILE* file;
	//std::fstream fs(path, std::fstream::in);
#ifdef __EMSCRIPTEN__
	file = fopen(path, "r");
#else
	fopen_s(&file, path, "r");
#endif // __EMSCRIPTEN__
	if (file == NULL)
	//if (fs.is_open() == NULL)
	{
		CLog::MyLog(LogType::Error, "Failed to load a model from %s", path);
		return;
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
#ifdef __EMSCRIPTEN__
		int res = fscanf(file, "%s", LineHeader);
#else
		int res = fscanf_s(file, "%s", LineHeader, sizeof(LineHeader)/sizeof(LineHeader[0]));
		//fs >> std::ws >> LineHeader;
#endif	
		if (res == NULL)
			break;

		if (strcmp(LineHeader, "v") == 0)
		{
			glm::vec3 Vertex;
#ifdef __EMSCRIPTEN__
			fscanf(file, "%f %f %f", &Vertex.x, &Vertex.y, &Vertex.z);
#else
			fscanf_s(file, "%f %f %f", &Vertex.x, &Vertex.y, &Vertex.z);
			//fs >> Vertex.x >> Vertex.y >> Vertex.z;
#endif
			temp_Vertices.push_back(Vertex);
		}
		else if (strcmp(LineHeader, "vt") == 0)
		{
			tempModel->HasTexcords = true;
			glm::vec2 Texcord;
#ifdef __EMSCRIPTEN__
			fscanf(file, "%f %f", &Texcord.x, &Texcord.y);
#else
			//fs >> Texcord.x >> Texcord.y;
			fscanf_s(file, "%f %f", &Texcord.x, &Texcord.y);
#endif
			Texcord.y = 1.f - Texcord.y;
			temp_Texcords.push_back(Texcord);
		}
		else if (strcmp(LineHeader, "vn") == 0)
		{
			tempModel->HasNormals = true;
			glm::vec3 Normal;
#ifdef __EMSCRIPTEN__
			fscanf(file, "%f %f %f", &Normal.x, &Normal.y, &Normal.z);
#else
			//fs >> Normal.x >> Normal.y >> Normal.z;
			fscanf_s(file, "%f %f %f", &Normal.x, &Normal.y, &Normal.z);
#endif
			temp_Normals.push_back(Normal);
		}
		else if (strcmp(LineHeader, "f") == 0)
		{
			if (tempModel->HasNormals && tempModel->HasTexcords)
			{
				unsigned int vertexIndex[5], uvIndex[5], normalIndex[5];
#ifdef __EMSCRIPTEN__
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
										&vertexIndex[0], &uvIndex[0], &normalIndex[0],
										&vertexIndex[1], &uvIndex[1], &normalIndex[1],
										&vertexIndex[2], &uvIndex[2], &normalIndex[2],
										&vertexIndex[3], &uvIndex[3], &normalIndex[3],
										&vertexIndex[4], &uvIndex[4], &normalIndex[4]);
#else
				int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
										&vertexIndex[0], &uvIndex[0], &normalIndex[0],
										&vertexIndex[1], &uvIndex[1], &normalIndex[1],
										&vertexIndex[2], &uvIndex[2], &normalIndex[2],
										&vertexIndex[3], &uvIndex[3], &normalIndex[3],
										&vertexIndex[4], &uvIndex[4], &normalIndex[4]);
#endif
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
#ifdef __EMSCRIPTEN__
				int matches = fscanf(file, "%d//%d %d//%d %d//%d %d//%d %d//%d\n",
										&vertexIndex[0], &normalIndex[0],
										&vertexIndex[1], &normalIndex[1],
										&vertexIndex[2], &normalIndex[2],
										&vertexIndex[3], &normalIndex[3],
										&vertexIndex[4], &normalIndex[4]);
#else
				int matches =  fscanf_s(file, "%d//%d %d//%d %d//%d %d//%d %d//%d\n",
										&vertexIndex[0], &normalIndex[0],
										&vertexIndex[1], &normalIndex[1],
										&vertexIndex[2], &normalIndex[2],
										&vertexIndex[3], &normalIndex[3],
										&vertexIndex[4], &normalIndex[4]);
#endif
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
#ifdef __EMSCRIPTEN__
				int matches = fscanf(file, "%d/%d %d/%d %d/%d %d/%d %d/%d\n",
										&vertexIndex[0], &uvIndex[0],
										&vertexIndex[1], &uvIndex[1],
										&vertexIndex[2], &uvIndex[2],
										&vertexIndex[3], &uvIndex[3],
										&vertexIndex[4], &uvIndex[4]);
#else
				int matches = fscanf_s(file, "%d/%d %d/%d %d/%d %d/%d %d/%d\n",
										&vertexIndex[0], &uvIndex[0],
										&vertexIndex[1], &uvIndex[1],
										&vertexIndex[2], &uvIndex[2],
										&vertexIndex[3], &uvIndex[3],
										&vertexIndex[4], &uvIndex[4]);
#endif
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

	// Changed indexes for 4850
	glGenVertexArrays(1, &tempModel->VAO);
	glBindVertexArray(tempModel->VAO);
	glGenBuffers(3, tempModel->VBOs);
//
//#ifdef HD4850 
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

	const char* name(path);
	const char* slash = strrchr(name, '/');
	const char* JustName(slash + 1);
	tempModel->Name = JustName;
	this->Models.push_back(tempModel);
}

std::shared_ptr<Model> CModelManager::GetModelByName(std::string name)
{
	for (auto o : this->Models)
	{
		if (strcmp(o->Name, name.c_str()) == 0)
		{
			return o;
		}
	}
	return {};
}
