#include "CTextureManager.h"

std::unique_ptr<CTextureManager> TextureManager;

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
}

std::shared_ptr<Texture> CTextureManager::LoadTexture(std::string path)
{
	SDL_Surface* Tex = IMG_Load(path.c_str());
	std::shared_ptr<Texture> TextureObject = std::make_shared<Texture>();

	char OnlyName[128];
	char* LastSlash;
	LastSlash = strrchr((char*)path.c_str(), '/');
#ifdef __EMSCRIPTEN__
	strcpy(OnlyName, LastSlash + 1);
#else
	strcpy_s(OnlyName, LastSlash + 1);
#endif

	auto ret = this->GetTextureByName(OnlyName);
	CLog::MyLog(LogType::Debug, "Texture name %s", OnlyName);
	if (ret != -1)
	{
		CLog::MyLog(LogType::Error, "Failed to load texture %s. File has been already loaded", path.c_str());
		return nullptr;
	}

	if (Tex == nullptr)
	{
		CLog::MyLog(LogType::Error, "Failed to load texture %s. File not found", path.c_str());
		return nullptr;
	}
	else
	{
		GLuint Format;
		SDL_Surface* FinalTex;
		if (Tex->format->BitsPerPixel == 24)
		{
			if (Tex->format->Rshift != 0)
			{
				FinalTex = SDL_ConvertSurfaceFormat(Tex, SDL_PIXELFORMAT_RGB24, 0);
				SDL_FreeSurface(Tex);
			}
			else
			{
				FinalTex = Tex;
			}
			Format = GL_RGB;
		}
		else
		{
			if (Tex->format->Rshift != 0)
			{
				FinalTex = SDL_ConvertSurfaceFormat(Tex, SDL_PIXELFORMAT_RGBA32, 0);
				SDL_FreeSurface(Tex);
			}
			else
			{
				FinalTex = Tex;
			}
			Format = GL_RGBA;
		}

		GLuint TexID;
		glGenTextures(1, &TexID);
		glBindTexture(GL_TEXTURE_2D, TexID);

		glTexImage2D(GL_TEXTURE_2D, 0, Format, FinalTex->w, FinalTex->h, 0, Format, GL_UNSIGNED_BYTE, FinalTex->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		TextureObject->name = std::string(OnlyName);
		TextureObject->texHandle = TexID;
		this->TextureVector.push_back(TextureObject);

		SDL_FreeSurface(FinalTex);
		return TextureObject;
	}
}

GLuint CTextureManager::GetTextureByName(std::string name)
{
	for (auto a : this->TextureVector)
	{
		if (strcmp(a->name.c_str(), name.c_str()) == 0)
		{
			return a->texHandle;
		}
	}
	return -1;
}