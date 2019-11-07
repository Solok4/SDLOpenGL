#pragma once
#include "CObject2D.h"
#include "SDL_ttf.h"
#include "CLog.h"
#include "GL/glew.h"
class CLabel :
	public CObject2D
{
public:
	CLabel();
	~CLabel();
	void SetFont(TTF_Font* font);
	void SetText(const char* format, ...);
	virtual void Prepare() override;
	TTF_Font* GetFont();
	void SetColorSDL(SDL_Color Col);


	TTF_Font* Font;
	SDL_Surface* TextSurface;

};

