#pragma once
#include "CLayout.h"
#include <map>

class CLayoutManager
{
public:
	CLayoutManager();
	~CLayoutManager();

	void AddNewLayout(const char* name);
	std::shared_ptr<CLayout> GetLayoutByName(const char* name);

	void ChangeCurrentLayout(const char* name);

	std::shared_ptr<CLayout> GetCurrentLayout();

	void RefreshWindowData();

private:

	std::shared_ptr<CLayout> CurrentLayout;

	std::vector<std::shared_ptr<CLayout>> Layouts;
};

extern std::unique_ptr<CLayoutManager> LayoutManager;
