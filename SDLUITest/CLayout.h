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
#include "CRenderer.h"

class CLayout
{
public:
	CLayout();
	~CLayout();
	void RefreshWindowData();
	void PrepareToLoop();
	void Draw();

	virtual void InitalizeLayout() {};

	void SetFont(const char* font);
	TTF_Font* GetFont();

	void AddItem(int id, const char* name,glm::vec2 pos,glm::vec2 size);
	template<typename T>
	std::shared_ptr<T> FindObjectByName(const char* name)
	{
		for (auto a : Objects2D)
		{
			if (strcmp(name, a->GetName()) == 0)
			{
				return std::dynamic_pointer_cast<T>(a);
			}
		}
		/*for (auto it = Objects2D.begin(); it != Objects2D.end(); ++it)
		{
			for (auto o : it->second)
			{
				if (strcmp(o->GetName(), name) == 0)
				{
					return std::dynamic_pointer_cast<T>(o);
				}
			}
		}*/
		return nullptr;
	}
	std::vector<std::shared_ptr<CObject2D>> GetObjectByType(int type);

	std::vector<std::shared_ptr<CObject2D>> GetButtons();
	void AddButtonToList(std::shared_ptr<CObject2D> btn);

	const char* GetName();
	void SetName(const char* name);

	virtual void Tick(double delta);

	std::shared_ptr<WindowInfo> WNDInfo;

	int MousePosX;
	int MousePosY;

	const char* Name;

	GLuint* ShaderProgram;
	TTF_Font* Font;

	//std::map<int,std::vector<std::shared_ptr<CObject2D>>> Objects2D;
	std::vector<std::shared_ptr<CObject2D>> Objects2D;
	std::vector<std::shared_ptr<CObject2D>> Buttons;

};


