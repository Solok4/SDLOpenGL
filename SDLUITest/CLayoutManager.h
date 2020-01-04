#pragma once
#include "CLayout.h"
#include <map>
#include <stack>

class CLayoutManager
{
public:
	CLayoutManager();
	~CLayoutManager();

	std::shared_ptr<CLayout> AddNewLayout(const char* name);
	std::shared_ptr<CLayout> GetLayoutByName(const char* name);

	void ChangeCurrentLayout(const char* name);

	void Draw();

	std::shared_ptr<CLayout> GetCurrentLayout();
	void PushActiveLayout(const char* name);
	void PopActiveLayout();

	void RefreshWindowData();

private:

	std::vector<std::shared_ptr<CLayout>> ActiveLayouts;
	std::vector<std::shared_ptr<CLayout>> Layouts;
};

extern std::unique_ptr<CLayoutManager> LayoutManager;
