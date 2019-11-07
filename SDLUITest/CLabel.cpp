#include "pch.h"
#include "CLabel.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <stdarg.h>


CLabel::CLabel()
{
	this->_Size = glm::vec2(200.0f, 100.0f);
	this->_Rotation = vec3(180.f, 0.0f, 0.0f);
	this->Font = TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 20);
	this->SetText("Default");
	this->ColorMask = vec4(1.0f);
}


CLabel::~CLabel()
{
}


void CLabel::SetFont(TTF_Font* font)
{
	this->Font = font;
}

void CLabel::SetText(const char* format, ...)
{
	char Text[512];
	va_list vl;
	va_start(vl, format);
#ifdef __EMSCRIPTEN__
	vsprintf(Text, format, vl);
#else
	vsprintf_s(Text, format, vl);
#endif
	if (!(TextSurface = TTF_RenderText_Blended(this->Font, Text, {255,255,255})))
	{
		CLog::MyLog(LogType::Error, "Failed to create text surface: %s",TTF_GetError());
		return;
	}
	else
	{
		this->SetSize(glm::vec2(TextSurface->w,TextSurface->h));
		glBindTexture(GL_TEXTURE_2D, this->TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextSurface->w, TextSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextSurface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		SDL_FreeSurface(TextSurface);
	}
	va_end(vl);
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

void CLabel::SetColorSDL(SDL_Color Col)
{
	this->SetColor(vec3(Col.b/255,Col.g/255,Col.r/255)) ;
}

