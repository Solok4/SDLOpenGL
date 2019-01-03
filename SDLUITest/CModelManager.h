#pragma once
#include <map>
#include "GL/glew.h"
#include <vector>
#include <memory>
#include "SDL_image.h"
#include "SDL.h"
#include <algorithm>
#include <thread>

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

	void LoadOBJ(const char* path, const char* tex);
	void Load(const char* path, const char* tex);
	void ThreadJoin();
	std::shared_ptr<Model> GetModelByName(std::string name);

private:
	std::map<std::string, std::shared_ptr<Model>> Models;
	std::vector<std::thread> Threads;
};


