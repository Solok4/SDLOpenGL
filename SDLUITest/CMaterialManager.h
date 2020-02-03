#pragma once
#include <string>
#include <cstring>
#include <vector>

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "CLog.h"
#include "CTextureManager.h"


enum TextureTypes
{
	BaseTex = 1,
	NormalMap=2,
	SpecularMap=4,
};

struct MaterialTexHandle
{
	MaterialTexHandle(GLuint tex, TextureTypes type) :TexHandle(tex), Type(type) {};
	GLuint TexHandle;
	TextureTypes Type;
};

struct LightMaterial
{
	LightMaterial() :Ambient(glm::vec3(1.0f, 0.5f, 0.31f)), Diffuse(glm::vec3(1.0f, 0.5f, 0.31f)), Specular(glm::vec3(1.0f, 0.5f, 0.31f)), Shininess(32.f) {};
	LightMaterial(glm::vec3 Amb, glm::vec3 Diff, glm::vec3 Spec, float Shinin) :Ambient(Amb), Diffuse(Diff), Specular(Spec), Shininess(Shinin) {};
	glm::vec3 Ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 Diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 Specular = glm::vec3(1.0f, 0.5f, 0.31f);
	float Shininess = 32.f;
};

class Material
{
public:
	Material(std::string name) :Name(name) { Textures.reserve(4); this->LightMat = std::make_shared<LightMaterial>(); };
	~Material() {};
	void AddTextureToMaterial(std::string name, TextureTypes type);
	std::string GetName() { return this->Name; };
	std::shared_ptr<LightMaterial> GetLightMaterial() { return this->LightMat; };
	GLuint GetTextureByType(TextureTypes type);

private:
	std::string Name;
	std::vector<std::shared_ptr<MaterialTexHandle>> Textures;
	std::shared_ptr<LightMaterial> LightMat;
};

class CMaterialManager
{
public:
	CMaterialManager();
	~CMaterialManager();

	std::shared_ptr<Material> CreateNewMaterial(std::string name);
	std::shared_ptr<Material> GetMaterialByName(std::string name);

private:
	std::vector<std::shared_ptr<Material>> MaterialVector;
};

extern std::unique_ptr<CMaterialManager> MaterialManager;