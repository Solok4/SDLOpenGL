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
	CLog::MyLog(0, "ModelManagerDestructor");
}

void CModelManager::LoadOBJ(const char * path, const char* tex)
{
	FILE* file;
	fopen_s(&file,path, "r");
	if (file == NULL)
	{
		CLog::MyLog(1, "Failed to load a model from " + std::string(path));
		assert(1 < 0);
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
					assert(1 < 0);
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
					assert(1 < 0);
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
					assert(1 < 0);
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

	auto ptr = &tempModel;

	if (ptr == nullptr)
	{
		CLog::MyLog(0, "ABC");
	}

	glGenVertexArrays(1, &tempModel->VAO);
	glBindVertexArray(tempModel->VAO);
	glGenBuffers(3, tempModel->VBOs);
	glGenBuffers(1, &tempModel->EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempModel->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, VertexIndices.size() * sizeof(unsigned int), &VertexIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, tempModel->VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, out_Vertices.size() * sizeof(glm::vec3), &out_Vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (tempModel->HasTexcords)
	{
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, tempModel->VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, out_Texcords.size() * sizeof(glm::vec2), &out_Texcords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glDisableVertexAttribArray(1);
	}

	if (tempModel->HasNormals)
	{
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, tempModel->VBOs[2]);
		glBufferData(GL_ARRAY_BUFFER, out_Normals.size() * sizeof(glm::vec3), &out_Normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glDisableVertexAttribArray(2);
	}



	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	
	SDL_Surface* Tex = IMG_Load(tex);
	if (!Tex)
	{
		CLog::MyLog(1, "Failed to load a texture");
	}
	else
	{
		GLint Format;
		if (Tex->format->BitsPerPixel == 32)
		{
			Format = GL_RGBA;
		}
		else if (Tex->format->BitsPerPixel == 24)
		{
			Format = GL_RGB;
		}
		GLuint TexID;
		glGenTextures(1, &TexID);
		glBindTexture(GL_TEXTURE_2D, TexID);

		glTexImage2D(GL_TEXTURE_2D, 0, Format, Tex->w, Tex->h, 0, Format, GL_UNSIGNED_BYTE, Tex->pixels);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		tempModel->Tex = TexID;
	}

	SDL_FreeSurface(Tex);



	std::string name(path);
	int slash = name.find_last_of("/");
	this->Models.emplace(name.substr(slash+1, slash + 2), tempModel);
}

void CModelManager::Load(const char * path, const char * tex)
{
	std::string Extension(path);
	std::string Name(path);
	int slash = Name.find_last_of('/');
	Name = Name.substr(slash + 1, slash + 2);
	auto Model = CModelManager::GetModelByName(Name);
	if (Model == nullptr)
	{
		int dot = Extension.find_last_of(".");
		Extension = Extension.substr(dot + 1, dot + 2);
		std::transform(Extension.begin(), Extension.end(), Extension.begin(), ::tolower);
		const char* Ex = Extension.c_str();
		if (strcmp(Ex, "obj") == 0)
		{
			std::thread t1([=] {this->LoadOBJ(path, tex); });
			this->Threads.push_back(std::move(t1));
		}
	}
}

void CModelManager::ThreadJoin()
{
	for (auto& o : Threads)
	{
		o.join();
	}
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
