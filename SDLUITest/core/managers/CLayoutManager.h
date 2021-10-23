#pragma once
#include "../../CLayout.h"
#include <map>
#include <stack>

class CLayoutManager
{
public:
	CLayoutManager();
	~CLayoutManager();

	//Adds new default layout to Layouts list and returns it
	std::shared_ptr<CLayout> AddNewLayout(const char* name);
	//Returns existing layout
	std::shared_ptr<CLayout> GetLayoutByName(const char* name);
	//Changes main layout
	void ChangeCurrentLayout(const char* name);
	//Calls draw for each active layouts available
	void Draw();
	//Returns layout on the end of the list. This layout will be available to use mouse events.
	std::shared_ptr<CLayout> GetCurrentLayout();
	//Pushes active layout. It allows to display more than one layout at once
	void PushActiveLayout(const char* name);
	//Removes last active layout.
	void PopActiveLayout();
	//Refreshes window data for all layouts.
	void RefreshWindowData();

private:

	std::vector<std::shared_ptr<CLayout>> ActiveLayouts;
	std::vector<std::shared_ptr<CLayout>> Layouts;
};

extern std::unique_ptr<CLayoutManager> LayoutManager;
