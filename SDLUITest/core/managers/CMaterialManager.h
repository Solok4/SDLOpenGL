#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <memory>
#include "../shared/Shared.h"
#include "glm.hpp"
#include "../../CLog.h"
#include "CTextureManager.h"

enum TextureTypes
{
	BaseTex = 0,
	NormalMap,
	SpecularMap,
};

struct MaterialTexHandle
{
	MaterialTexHandle(GLuint tex, TextureTypes type) :TexHandle(tex), Type(type) {};
	GLuint TexHandle;
	TextureTypes Type;
};

struct MaterialData {
	MaterialData() {
		this->AmbientColor = glm::vec3(1.0f, 0.5f, 0.31f);
		this->DiffuseColor = glm::vec3(1.0f, 0.5f, 0.31f);
		this->SpecularColor = glm::vec3(1.0f, 0.5f, 0.31f);
		this->Transparency = 1.0f;
		this->IluminationModel = 2;
		this->Shininess = 32.f;
		this->RefractionIndex = 1.0f;
		this->DiffuseTex = -1;
		this->SpecularTex = -1;
		this->NormalTex = -1;
	}
	glm::vec3 AmbientColor;
	glm::vec3 DiffuseColor;
	glm::vec3 SpecularColor;
	float Transparency;
	uint32_t IluminationModel;
	float Shininess;
	float RefractionIndex;

	GLuint DiffuseTex;
	GLuint SpecularTex;
	GLuint NormalTex;
};

class Material
{
public:
	Material(std::string name);
	~Material() {};
	void AddTextureToMaterial(std::string name, TextureTypes type);
	std::string GetName() { return this->Name; };
	GLuint GetTextureByType(TextureTypes type);
	std::shared_ptr<MaterialData> GetData();

private:
	std::string Name;
	std::shared_ptr<MaterialData> Data;
};

class CMaterialManager
{
public:
	CMaterialManager();
	~CMaterialManager();

	std::shared_ptr<Material> CreateNewMaterial(std::string name);
	std::vector<std::shared_ptr<Material>> CreateNewMaterialFromFile(std::string path);
	std::shared_ptr<Material> GetMaterialByName(std::string name);
	std::shared_ptr<Material> GetDefaultMaterial();

private:
	std::vector<std::shared_ptr<Material>> MaterialVector;
	std::shared_ptr<Material> DefaultMaterial;
};

extern std::unique_ptr<CMaterialManager> MaterialManager;