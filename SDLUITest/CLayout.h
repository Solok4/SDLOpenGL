#pragma once

#include <vector>
#include "CObject2D.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "CButton.h"
#include "CLabel.h"
#include "CImage.h"
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
	void AddItem(int id,std::string name,glm::vec2 pos,glm::vec2 size);
	CObject2D* FindObjectByName(std::string name);
	std::vector<CObject2D*> GetObjectByType(int type);

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

enum Object2DType
{
	OBJECT2D_LABEL=0,
	OBJECT2D_IMAGE,
	OBJECT2D_BUTTON
};

