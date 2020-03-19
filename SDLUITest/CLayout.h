#pragma once

#include <map>
#include "SDL.h"
#include "SDL_ttf.h"

#include "COpengl.h"
#include "CRenderer.h"
#include "CLog.h"

#include "CObject2D.h"
#include "CButton.h"
#include "CLabel.h"
#include "CImage.h"
#include "CContainer.h"
#include "CTextBox.h"
#include "CListBox.h"

class CLayout
{
public:
	CLayout();
	~CLayout();
	void RefreshWindowData();
	void PrepareToLoop();
	void Draw();

	//It will create all layout elements.
	virtual void InitalizeLayout() {};

	//Adds item to the layout.
	void AddItem(int id, const char* name,glm::vec2 pos,glm::vec2 size);
	//Retuns layout element by name and type.
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
		return nullptr;
	}
	//Returns button list in the layout
	std::vector<std::shared_ptr<CObject2D>> GetButtons();
	//Adds button to button list
	void AddButtonToList(std::shared_ptr<CObject2D> btn);
	//Returns name of the layout
	const char* GetName();
	//Sets name of the layout
	void SetName(const char* name);
	//Function called every frame
	virtual void Tick(double delta);

	void ProcessTextEditing();

	void SetUsedTextBox(CTextBox* tbox);
	bool GetIsEditingLayout() { return this->LayoutIsEditing; };
	void RefreshScreenSize();

private:
	int MousePosX;
	int MousePosY;
	const char* Name;
	std::shared_ptr<WindowInfo> WNDInfo;
	std::vector<std::shared_ptr<CObject2D>> Objects2D;
	std::vector<std::shared_ptr<CObject2D>> Buttons;
	bool LayoutIsEditing = false;
	CTextBox* UsedTestBox;
	std::shared_ptr<CContainer> RootContainer;
};


