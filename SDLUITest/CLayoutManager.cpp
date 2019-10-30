#include "pch.h"
#include "CLayoutManager.h"

std::unique_ptr<CLayoutManager> LayoutManager;

CLayoutManager::CLayoutManager()
{
	this->AddNewLayout("Blank");
	this->ChangeCurrentLayout("Blank");
}


CLayoutManager::~CLayoutManager()
{
	CLog::MyLog(LogType::Log, "LayoutManagerDestructor");
}

void CLayoutManager::AddNewLayout(const char* name)
{
	std::shared_ptr<CLayout> temp(new CLayout);
	temp->SetName(name);

	this->Layouts.push_back(temp);
}

std::shared_ptr<CLayout> CLayoutManager::GetLayoutByName(const char* name)
{
	for (auto o : this->Layouts)
	{
		if (strcmp(o->GetName(),name) == 0)
		{
			return o;
		}
	}
	return nullptr;
}

void CLayoutManager::ChangeCurrentLayout(const char* name)
{
	for (auto o : this->Layouts)
	{
		if (strcmp(o->GetName(), name) == 0)
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
	if(this->CurrentLayout !=nullptr)
		this->CurrentLayout->GetMousePosition(mX, mY);
}

void CLayoutManager::SetWindowData(SDL_Window * Window)
{
	for (auto o : this->Layouts)
	{
		o->SetWindowData(Window);
	}
}
