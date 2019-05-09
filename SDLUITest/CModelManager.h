#pragma once
#include <map>
#include "GL/glew.h"
#include <vector>
#include <memory>
#include "SDL_image.h"
#ifdef __EMSCRIPTEN__
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif // __EMSCRIPTEN__
#include "glm/glm.hpp"





struct LightMaterial
{
	glm::vec3 Ambient = glm::vec3(1.0f,0.5f,0.31f);
	glm::vec3 Diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 Specular = glm::vec3(1.0f, 0.5f, 0.31f);
	float Shininess =32.f;
};

struct Material
{
	std::vector<GLuint> Tex;
	GLubyte BaseTexIndex =255;
	GLubyte NormalMapIndex=255;
	GLubyte SpecularMapIndex=255;
	LightMaterial LM;
	const char* Name;
};

struct Model
{
	bool HasTexcords = false;
	bool HasNormals = false;
	unsigned int IndicesCount = 0;
	GLuint VAO;
	GLuint VBOs[3];
	std::shared_ptr<Material> Mat = nullptr;
	const char* Name;
};

enum TextureTypes
{
	BaseTex=0,
	NormalMap,
	SpecularMap,
};

struct Texture
{
	GLuint Texture;
	const char* Name;
};


class CModelManager
{
public:
	CModelManager();
	~CModelManager();

	void LoadOBJ(const char* path);
	void Load(const char* path, const char* tex);
	void LoadTexture(const char* path);
	GLuint GetImageByName(const char* name);
	void CreateMaterial(const char* name);
	std::shared_ptr<Material> GetMaterialByName(const char* name);
	void BindTextureToMaterial(const char* MaterialName, const char* TextureName, TextureTypes TextureType);
	std::shared_ptr<Model> GetModelByName(std::string name);

private:

	std::vector<std::shared_ptr<Model>> Models;
	std::vector<std::shared_ptr<Material>> Materials;
	std::vector<std::shared_ptr<Texture>> Textures;
};


