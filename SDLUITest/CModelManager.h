#pragma once
#include <map>
#include "GL/glew.h"
#include <vector>
#include <memory>
#include <string>
#include "SDL_image.h"
#ifdef __EMSCRIPTEN__
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif // __EMSCRIPTEN__
#include "glm/glm.hpp"
#include "Shared.h"
#include "CMaterialManager.h"



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


class CModelManager
{
public:
	CModelManager();
	~CModelManager();

	void LoadOBJ(const char* path);

	/*void BindTextureToMaterial(const char* MaterialName, const char* TextureName, TextureTypes TextureType);*/
	std::shared_ptr<Model> GetModelByName(std::string name);

private:

	std::vector<std::shared_ptr<Model>> Models;
};

extern std::unique_ptr<CModelManager> ModelManager;

