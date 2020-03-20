#pragma once
#include <map>
#include <vector>
#include <string>
#include "SDL_ttf.h"
#include "CLog.h"

struct MyFont
{
	MyFont(TTF_Font* font, int size) :FontHandle(font), FontSize(size) {};
	TTF_Font* FontHandle;
	int FontSize;
};

class CFontManager
{
public:
	CFontManager();
	~CFontManager();

	TTF_Font* LoadFont(std::string path, int size);
	TTF_Font* GetFontByName(std::string name, int size);


	std::map<std::string, std::vector<MyFont>> FontMap;
};

extern std::unique_ptr<CFontManager> FontManager;
