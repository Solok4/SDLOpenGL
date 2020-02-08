#pragma once
#include <string>
#include <vector>
#include <cstring>

#include "GL/glew.h"
#include "SDL_image.h"

#include "CLog.h"

enum SkyboxType
{
	CubeType = 0,
};

struct Texture
{
	std::string name;
	GLuint texHandle;
};

class CTextureManager
{
public:
	CTextureManager();
	~CTextureManager();

	void LoadTexture(std::string path);
	GLuint GetTextureByName(std::string name);

	std::vector<std::shared_ptr<Texture>> TextureVector;
};

extern std::unique_ptr<CTextureManager> TextureManager;

