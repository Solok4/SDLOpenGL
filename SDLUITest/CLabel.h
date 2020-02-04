#pragma once
#include "CObject2D.h"
#include "SDL_ttf.h"
#include "CLog.h"
#include "GL/glew.h"
class CLabel :
	public CObject2D
{
public:
	CLabel()=delete;
	CLabel(const char* name, /*CLayout* ref,*/ glm::vec2 position, glm::vec2 objsize) :CObject2D(name, /*ref,*/ position, objsize) 
	{ 
		this->_ID = Object2DType::OBJECT2D_LABEL;
		this->_Rotation = glm::vec3(180.f, 0.0f, 0.0f);
	}
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

	//Prepares object to draw.
	virtual void PreDraw();
	//Draws the object. Should be used after PreDraw func.
	virtual void Draw();
	//Cleans up things after drawing.
	virtual void PostDraw();

private:
	TTF_Font* Font;
	SDL_Surface* TextSurface;
	GLuint TextureID;

};

