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
	//Sets font that will be used in label
	void SetFont(TTF_Font* font);
	//Creates texture with provided text
	void SetText(const char* format, ...);
	//Prepares label for using
	virtual void Prepare() override;
	//Returns font used by this label
	TTF_Font* GetFont();
	//Sets colour of the text
	void SetColorSDL(SDL_Color Col);

private:
	TTF_Font* Font;
	SDL_Surface* TextSurface;

};

