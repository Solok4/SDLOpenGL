#pragma once

#include <map>
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
	void PrepareToLoop();
	void Draw(COpengl* opengl);

	void SetFont(const char* font);
	TTF_Font* GetFont();

	void AddItem(int id, const char* name,glm::vec2 pos,glm::vec2 size);
	std::shared_ptr<CObject2D> FindObjectByName(const char* name);
	std::vector<std::shared_ptr<CObject2D>> GetObjectByType(int type);

	const char* GetName();
	void SetName(const char* name);

	void GetMousePosition(int x,int y);

	void Tick(uint32_t delta);

	int WindowWidth;
	int WindowHeight;
	int WindowPosX;
	int WindowPosY;

	int MousePosX;
	int MousePosY;

	const char* Name;

	GLuint* ShaderProgram;
	TTF_Font* Font;

	std::map<int,std::vector<std::shared_ptr<CObject2D>>> Objects2D;

};

enum Object2DType
{
	OBJECT2D_LABEL=0,
	OBJECT2D_IMAGE,
	OBJECT2D_BUTTON
};

