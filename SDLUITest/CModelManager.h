#pragma once
#include <map>
#include "GL/glew.h"
#include <vector>
#include <memory>
#include "SDL_image.h"
#include "SDL.h"

struct Model
{
	bool HasTexcords = false;
	bool HasNormals = false;
	unsigned int IndicesCount = 0;
	GLuint VAO;
	GLuint VBOs[2];
	GLuint EBO;
	GLuint Tex;
};


class CModelManager
{
public:
	CModelManager();
	~CModelManager();

	int LoadOBJ(const char* path, const char* tex);
	std::shared_ptr<Model> GetModelByName(std::string name);

private:
	std::map<std::string, std::shared_ptr<Model>> Models;
};


