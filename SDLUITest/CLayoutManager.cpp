#include "pch.h"
#include "CLayoutManager.h"


CLayoutManager::CLayoutManager()
{
}


CLayoutManager::~CLayoutManager()
{
}

void CLayoutManager::AddNewLayout(std::string name)
{
	CLayout* temp = new CLayout();

	this->Layouts.emplace(name, temp);
}

std::shared_ptr<CLayout> CLayoutManager::GetLayoutByName(std::string name)
{
	for (std::map<std::string, std::shared_ptr<CLayout>>::iterator it = Layouts.begin(); it != Layouts.end(); ++it)
	{
		if (it->first == name)
		{
			return it->second;
		}
	}
	return nullptr;
}

void CLayoutManager::ChangeCurrentLayout(std::string name)
{
	for (std::map<std::string, std::shared_ptr<CLayout>>::iterator it = Layouts.begin(); it != Layouts.end(); ++it)
	{
		if (it->first == name)
		{
			this->CurrentLayout = it->second;
		}
	}
}

std::shared_ptr<CLayout> CLayoutManager::GetCurrentLayout()
{
	return this->CurrentLayout;
}

void CLayoutManager::SetMousePosition(int mX, int mY)
{
	this->CurrentLayout->GetMousePosition(mX, mY);
}
