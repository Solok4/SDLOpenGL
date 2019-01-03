#pragma once
#include "CLayout.h"
#include <map>

class CLayoutManager
{
public:
	CLayoutManager();
	~CLayoutManager();

	void AddNewLayout(std::string name);
	std::shared_ptr<CLayout> GetLayoutByName(std::string name);

	void ChangeCurrentLayout(std::string name);

	std::shared_ptr<CLayout> GetCurrentLayout();

	void SetMousePosition(int mX, int mY);

private:

	std::shared_ptr<CLayout> CurrentLayout;

	std::vector<std::shared_ptr<CLayout>> Layouts;
};

