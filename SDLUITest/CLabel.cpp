#include "pch.h"
#include "CLabel.h"
#include "gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"


CLabel::CLabel()
{
	this->_Size = glm::vec2(200.0f, 100.0f);
	this->Color = { 255,255,255 };
	this->_Rotation = vec3(180.f, 0.0f, 0.0f);
	this->Font = TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 20);
	this->SetText("Default");
}


CLabel::~CLabel()
{
}


void CLabel::SetFont(TTF_Font* font)
{
	this->Font = font;
}

void CLabel::SetText(const char * Text)
{
	SDL_Surface* TextSurface;
	if (!(TextSurface = TTF_RenderText_Blended(this->Font, Text, this->Color)))
	{
		CLog::MyLog(1, "Failed to create text surface: " + std::string(TTF_GetError()));
		return;
	}
	else
	{
		TextSurface = SDL_ConvertSurfaceFormat(TextSurface, SDL_PIXELFORMAT_RGBA32, 0);
		this->SetSize(glm::vec2(TextSurface->w,TextSurface->h));
		glBindTexture(GL_TEXTURE_2D, this->TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextSurface->w, TextSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextSurface->pixels);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		SDL_FreeSurface(TextSurface);
	}
}

void CLabel::Prepare()
{
	CObject2D::Prepare();
	glGenTextures(1, &this->TextureID);
}

TTF_Font * CLabel::GetFont()
{
	return this->Font;
}

void CLabel::SetColor(SDL_Color Col)
{
	this->Color = { Col.b ,Col.g,Col.r };
}

