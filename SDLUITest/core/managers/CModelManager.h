#pragma once
#include <map>
#include <vector>
#include <memory>
#include <string>
#include "SDL_image.h"
#include "glm.hpp"
#include "../shared/Shared.h"
#include "CMaterialManager.h"

struct ModelData {
	const char* Name;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCords;
	std::vector<glm::vec3> normals;
	bool HasTexcords = false;
	bool HasNormals = false;
	unsigned int indicesCount;
};

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
			size = this->modelData->indicesCount;
		}
		this->RenderingChunks.push_back(std::make_shared<RenderingChunk>(mat, offset, size));
	}
	GLuint VAO;
	GLuint VBOs[3];
	std::vector<std::shared_ptr<RenderingChunk>> RenderingChunks;
	std::shared_ptr<ModelData> modelData;
};

class CModelManager
{
public:
	CModelManager();
	~CModelManager();

	void LoadOBJ(const char* path);
	std::shared_ptr<Model> GetModelByName(std::string name);

private:
	std::shared_ptr<ModelData> LoadObjFromFile(const char* path);
	void prepareModelForRendering(std::shared_ptr<ModelData>);

	std::vector<std::shared_ptr<ModelData>> loadedModels;
	std::vector<std::shared_ptr<Model>> Models;
};

extern std::unique_ptr<CModelManager> ModelManager;
