#pragma once

#include <vector>
#include "CObject2D.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "CButton.h"
#include "CLabel.h"
#include "CLog.h"
#include "COpengl.h"
class CLayout
{
public:
	CLayout();
	~CLayout();
	void SetWindowData(SDL_Window* WND);
	void SetShaderProgram(GLuint* Program);
	void PrepareToLoop();
	void Draw(COpengl* opengl);
	void SetFont(const char* font);
	TTF_Font* GetFont();
	void AddItem(int id,glm::vec2 pos,glm::vec2 size);

	void GetMousePosition(int x,int y);

	int WindowWidth;
	int WindowHeight;
	int WindowPosX;
	int WindowPosY;

	int MousePosX;
	int MousePosY;

	GLuint* ShaderProgram;
	TTF_Font* Font;

	std::vector<CObject2D*> Objects2D;

};

