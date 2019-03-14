#include "pch.h"
#include "CModelManager.h"
#include <fstream>
#include "CLog.h"
#include "glm/glm.hpp"


CModelManager::CModelManager()
{
}


CModelManager::~CModelManager()
{
	CLog::MyLog(0, "ModelManagerDestructor");
}

void FinishSurface(SDL_Surface* Surface)
{
	SDL_LockSurface(Surface);

	unsigned int* Pixels = (unsigned int*)Surface->pixels;
	unsigned int* FinalPixels = (unsigned int*)malloc(Surface->pitch*Surface->h);
	size_t PixLen = strlen((char*)Pixels);
	size_t FPixLen = strlen((char*)FinalPixels);

	for (int y = 0; y < Surface->h; y++)
	{
		for (int x = 0; x < Surface->w; x++)
		{
			FinalPixels[y*Surface->w + x] = Pixels[(Surface->h - y)*Surface->w + x];
		}
	}
	for (int y = 0; y < Surface->h; y++)
	{
		for (int x = 0; x < Surface->w; x++)
		{
			Pixels[y*Surface->w + x] = FinalPixels[y*Surface->w + x];
		}
	}
	SDL_UnlockSurface(Surface);
	free(FinalPixels);
}

void CModelManager::LoadOBJ(const char * path)
{
	FILE* file;
#ifdef __EMSCRIPTEN__
	file = fopen(path, "r");
	if (file == NULL)
	{
		CLog::MyLog(1, "Failed to load a model from %s", path);
		return;
	}
#else
	fopen_s(&file, path, "r");
	if (file == NULL)
	{
		CLog::MyLog(1, "Failed to load a model from %s", path);
		return;
	}
#endif // __EMSCRIPTEN__

	
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
#ifdef __EMSCRIPTEN__
		char LineHeader[128];
		int res = fscanf(file, "%s", LineHeader);
		if (res == EOF)
			break;

		if (strcmp(LineHeader, "v") == 0)
		{
			glm::vec3 Vertex;
			fscanf(file, "%f %f %f", &Vertex.x, &Vertex.y, &Vertex.z);
			temp_Vertices.push_back(Vertex);
		}
		else if (strcmp(LineHeader, "vt") == 0)
		{
			tempModel->HasTexcords = true;
			glm::vec2 Texcord;
			fscanf(file, "%f %f", &Texcord.x, &Texcord.y);
			temp_Texcords.push_back(Texcord);
		}
		else if (strcmp(LineHeader, "vn") == 0)
		{
			tempModel->HasNormals = true;
			glm::vec3 Normal;
			fscanf(file, "%f %f %f", &Normal.x, &Normal.y, &Normal.z);
			temp_Normals.push_back(Normal);
		}
		else if (strcmp(LineHeader, "f") == 0)
		{
			if (tempModel->HasNormals && tempModel->HasTexcords)
			{
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					fclose(file);
					return;
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
				int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
				if (matches != 6) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					fclose(file);
					return;
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
				int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
				if (matches != 6) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					fclose(file);
					return;
				}
				VertexIndices.push_back(vertexIndex[0]);
				VertexIndices.push_back(vertexIndex[1]);
				VertexIndices.push_back(vertexIndex[2]);
				TexcordIndices.push_back(uvIndex[0]);
				TexcordIndices.push_back(uvIndex[1]);
				TexcordIndices.push_back(uvIndex[2]);
			}


		}
#else
		char LineHeader[128];
		int res = fscanf_s(file, "%s", LineHeader, sizeof(LineHeader));
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
					fclose(file);
					return;
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
			//else if (tempModel->HasNormals && !tempModel->HasTexcords)
			//{
			//	unsigned int vertexIndex[3], normalIndex[3];
			//	int matches = fscanf_s(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
			//	if (matches != 6) {
			//		printf("File can't be read by our simple parser : ( Try exporting with other options\n");
			//		fclose(file);
			//		return;
			//	}
			//	VertexIndices.push_back(vertexIndex[0]);
			//	VertexIndices.push_back(vertexIndex[1]);
			//	VertexIndices.push_back(vertexIndex[2]);
			//	NormalIndices.push_back(normalIndex[0]);
			//	NormalIndices.push_back(normalIndex[1]);
			//	NormalIndices.push_back(normalIndex[2]);
			//}
			else if (!tempModel->HasNormals && tempModel->HasTexcords)
			{
				unsigned int vertexIndex[3], uvIndex[3];
				int matches = fscanf_s(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
				if (matches != 6) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					fclose(file);
					return;
				}
				VertexIndices.push_back(vertexIndex[0]);
				VertexIndices.push_back(vertexIndex[1]);
				VertexIndices.push_back(vertexIndex[2]);
				TexcordIndices.push_back(uvIndex[0]);
				TexcordIndices.push_back(uvIndex[1]);
				TexcordIndices.push_back(uvIndex[2]);
			}


		}

#endif // !__EMSCRIPTEN__

		
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
	glGenBuffers(2, tempModel->VBOs);
	/*glGenBuffers(1, &tempModel->EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempModel->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, VertexIndices.size() * sizeof(unsigned int), &VertexIndices[0], GL_STATIC_DRAW);*/

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

	//if (tempModel->HasNormals)
	//{
	//	glEnableVertexAttribArray(2);
	//	glBindBuffer(GL_ARRAY_BUFFER, tempModel->VBOs[2]);
	//	glBufferData(GL_ARRAY_BUFFER, out_Normals.size() * sizeof(glm::vec3), &out_Normals[0], GL_STATIC_DRAW);
	//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//	glDisableVertexAttribArray(2);
	//}

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	const char* name(path);
	const char* slash = strrchr(name, '/');
	const char* JustName(slash + 1);
	tempModel->Name = JustName;
	this->Models.push_back(tempModel);
}


void CModelManager::Load(const char * path, const char * tex)
{
}

void CModelManager::LoadTexture(const char * path)
{
	SDL_Surface* Tex = IMG_Load(path);
	std::shared_ptr<Texture> tempTex(new Texture);
	if (!Tex)
	{
		CLog::MyLog(1, "Failed to load a texture");
	}
	else
	{
		GLint Format;
		Format = GL_RGBA;
		SDL_Surface* Finish = SDL_ConvertSurfaceFormat(Tex, SDL_PIXELFORMAT_RGBA32, 0);
		FinishSurface(Finish);

		GLuint TexID;
		glGenTextures(1, &TexID);
		glBindTexture(GL_TEXTURE_2D, TexID);

		glTexImage2D(GL_TEXTURE_2D, 0, Format, Finish->w, Finish->h, 0, Format, GL_UNSIGNED_BYTE, Finish->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		tempTex->Texture = TexID;
		SDL_FreeSurface(Finish);
	}

	SDL_FreeSurface(Tex);

	const char* name(path);
	const char* slash = strrchr(name, '/');
	const char* JustName(slash + 1);
	tempTex->Name = JustName;
	this->Textures.push_back(tempTex);
}

GLuint CModelManager::GetImageByName(const char * name)
{
	for (auto o : this->Textures)
	{
		if (strcmp(name, o->Name) == 0)
		{
			return o->Texture;
		}
	}
	return -1;
}

void CModelManager::CreateMaterial(const char * name)
{
	std::shared_ptr<Material> temp(new Material);
	temp->Name = name;
	this->Materials.push_back(temp);
}

std::shared_ptr<Material> CModelManager::GetMaterialByName(const char * name)
{
	for (auto o : this->Materials)
	{
		if (strcmp(name, o->Name) == 0)
		{
			return o;
		}
	}
	return nullptr;
}

void CModelManager::BindTextureToMaterial(const char * MaterialName, const char * TextureName, TextureTypes TextureType)
{
	std::shared_ptr<Material> temp = this->GetMaterialByName(MaterialName);
	if (temp == nullptr)
	{
		CLog::MyLog(1, "Material named %s don't exist\n", MaterialName);
		return;
	}
	GLuint tempTex = this->GetImageByName(TextureName);
	if (tempTex == -1)
	{
		CLog::MyLog(1, "Texture named %s don't exist\n", TextureName);
		return;
	}
	GLubyte index = temp->Tex.size();
	temp->Tex.push_back(tempTex);
	switch (TextureType)
	{
	case BaseTex:
		temp->BaseTexIndex = index;
		break;
	case NormalMap:
		temp->NormalMapIndex = index;
		break;
	case SpecularMap:
		temp->SpecularMapIndex = index;
		break;
	}
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
