#include "pch.h"
#include "CLabel.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <stdarg.h>



CLabel::~CLabel()
{
	glDeleteTextures(1, &this->TextureID);
}


void CLabel::SetFont(TTF_Font* font)
{
	this->Font = font;
}

void CLabel::SetText(const char* format, ...)
{
	char Text[256];
	va_list vl;
	va_start(vl, format);
#ifdef __EMSCRIPTEN__
	vsprintf(Text, format, vl);
#else
	vsprintf_s(Text, format, vl);
#endif
	this->Value = std::string(Text);
	if (!(TextSurface = TTF_RenderText_Blended(this->Font, this->Value.c_str(), {255,255,255})))
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

void CLabel::SetTextWrapped(int width, const char* format, ...)
{
	char Text[256];
	va_list vl;
	va_start(vl, format);
#ifdef __EMSCRIPTEN__
	vsprintf(Text, format, vl);
#else
	vsprintf_s(Text, format, vl);
#endif
	this->Value = std::string(Text);
	if (!(TextSurface = TTF_RenderText_Blended_Wrapped(this->Font,this->Value.c_str(),{255,255,255},width)))
	{
		CLog::MyLog(LogType::Error, "Failed to create text surface: %s", TTF_GetError());
		return;
	}
	else
	{
		this->SetSize(glm::vec2(TextSurface->w, TextSurface->h));
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

void CLabel::PreDraw()
{
	if (this->_IsVisible)
	{
		glBindVertexArray(this->_VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glActiveTexture(GL_TEXTURE0);
	}

}

void CLabel::Draw()
{
	if (this->_IsVisible)
	{
		glBindTexture(GL_TEXTURE_2D, this->TextureID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void CLabel::PostDraw()
{
	if (this->_IsVisible)
	{
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}

