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

std::shared_ptr<CLayout> CLayoutManager::AddNewLayout(const char* name)
{
	std::shared_ptr<CLayout> temp = std::make_shared<CLayout>();
	temp->SetName(name);
	this->Layouts.push_back(temp);
	return temp;
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
			this->ActiveLayouts.clear();
			this->ActiveLayouts.push_back(o);
		}
	}
}

void CLayoutManager::Draw()
{
	for (int i = 0; i < this->ActiveLayouts.size(); i++)
	{
		this->ActiveLayouts[i]->Draw();
	}
}

std::shared_ptr<CLayout> CLayoutManager::GetCurrentLayout()
{
	if(!this->ActiveLayouts.empty())
		return this->ActiveLayouts[this->ActiveLayouts.size()-1];
}

void CLayoutManager::PushActiveLayout(const char* name)
{
	for (auto a : Layouts)
	{
		if (strcmp(name, a->GetName()) == 0)
		{
			this->ActiveLayouts.push_back(a);
		}
	}
}

void CLayoutManager::PopActiveLayout()
{
	if (!this->ActiveLayouts.empty())
	{
		this->ActiveLayouts.erase((this->ActiveLayouts.begin()+this->ActiveLayouts.size()-1));
	}
}

void CLayoutManager::RefreshWindowData()
{
	for (auto o : this->Layouts)
	{
		o->RefreshWindowData();
	}
}
