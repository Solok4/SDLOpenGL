#pragma once
#include "CObject2D.h"
#include "SDL_ttf.h"
#include "CLog.h"
#include "GL/glew.h"
class CLabel :
	public CObject2D
{
public:
	CLabel(unsigned int ID);
	~CLabel();
	void Draw() override;
	void SetFont(TTF_Font* font);
	void SetText(const char* Text);
	void SetColor(SDL_Color Col);


	TTF_Font* Font;
	SDL_Color Color;

};

