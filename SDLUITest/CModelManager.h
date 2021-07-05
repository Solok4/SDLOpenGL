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

struct RenderingChunk {
	RenderingChunk(std::shared_ptr<Material> mat, int off, int size) {
		this->material = mat->GetData();
		this->chunkOffset = off;
		this->chunkSize = size;
	}
	int chunkOffset = 0;
	int chunkSize = 0;
	std::shared_ptr<MaterialData> material;
};

struct Model
{
	void BindMaterialToModel(std::shared_ptr<Material> mat, int offset = 0, unsigned int size = 0) {
		if (size == 0) {
			size = this->IndicesCount;
		}
		this->RenderingChunks.push_back(std::make_shared<RenderingChunk>(mat, offset, size));
	}
	bool HasTexcords = false;
	bool HasNormals = false;
	unsigned int IndicesCount = 0;
	GLuint VAO;
	GLuint VBOs[3];
	std::vector<std::shared_ptr<RenderingChunk>> RenderingChunks;
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
