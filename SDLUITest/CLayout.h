#pragma once

#include <map>
#include "SDL.h"
#include "SDL_ttf.h"

#include "core/renderer/COpengl.h"
#include "core/managers/CWindowManager.h"
#include "CLog.h"

#include "core/widgets/CObject2D.h"
#include "core/widgets/CButton.h"
#include "core/widgets/CLabel.h"
#include "core/widgets/CImage.h"
#include "core/widgets/CContainer.h"
#include "core/widgets/CTextBox.h"
#include "core/widgets/CListBox.h"

#include "core/shared/Shared.h"

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
	void AddItem(int id, const char* name, glm::vec2 pos, glm::vec2 size);
	//Retuns layout element by name and type.
	template<typename T>
	std::shared_ptr<T> FindObjectByName(const char* name)
	{
		for (auto a : Objects2D)
		{
			if (StdLibWrapper::Sstrcmp(name, a->GetName()) == 0)
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
