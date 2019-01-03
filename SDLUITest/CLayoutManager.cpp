#include "pch.h"
#include "CLayoutManager.h"


CLayoutManager::CLayoutManager()
{
	this->AddNewLayout("Blank");
	this->ChangeCurrentLayout("Blank");
}


CLayoutManager::~CLayoutManager()
{
	CLog::MyLog(0, "LayoutManagerDestructor");
}

void CLayoutManager::AddNewLayout(std::string name)
{
	std::shared_ptr<CLayout> temp(new CLayout);
	temp->SetName(name);

	this->Layouts.push_back(temp);
}

std::shared_ptr<CLayout> CLayoutManager::GetLayoutByName(std::string name)
{
	for (auto o : this->Layouts)
	{
		if (o->GetName() == name)
		{
			return o;
		}
	}
	return nullptr;
}

void CLayoutManager::ChangeCurrentLayout(std::string name)
{
	for (auto o : this->Layouts)
	{
		if (o->GetName()==name)
		{
			this->CurrentLayout = o;
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
