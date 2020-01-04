#include "pch.h"
#include "CLayout.h"
#include <memory>
#include "COpengl.h"
#include "CEvent.h"



CLayout::CLayout()
{
	this->RefreshWindowData();
}


CLayout::~CLayout()
{
	CLog::MyLog(LogType::Log, "LayoutDestructor %s",this->Name);
}

void CLayout::RefreshWindowData()
{
	this->WNDInfo = Renderer->GetWindowInfo();
}

void CLayout::PrepareToLoop()
{
	for (auto a : Objects2D)
	{
		a->Prepare();
	}
	/*for(auto it = Objects2D.begin();it != Objects2D.end();++it)
	{
		for (auto o : it->second)
		{
			o->Prepare();
		}
	}*/
}

void CLayout::Draw()
{
	glDisable(GL_CULL_FACE);

	for (auto a : Objects2D)
	{
		a->PreDraw();
		OpenGL->SetModelMatrixLayout(a->GetModelMatrix());
		OpenGL->SetColorMaskLayout(a->GetColorMask());
		a->Draw();
		a->PostDraw();
	}
	/*for (auto it = Objects2D.begin(); it != Objects2D.end(); ++it)
	{	
		for (auto o : it->second)
		{
			o->PreDraw();
			OpenGL->SetModelMatrixLayout(o->GetModelMatrix());
			OpenGL->SetColorMaskLayout(o->GetColorMask());
			o->Draw();
			o->PostDraw();
			if (o->GetID() == Object2DType::OBJECT2D_BUTTON)
			{
				auto temp = std::dynamic_pointer_cast<CButton>(o);
				
				temp->GetLabel()->PreDraw();
				OpenGL->SetModelMatrixLayout(temp->GetLabel()->GetModelMatrix());
				temp->GetLabel()->Draw();
				temp->GetLabel()->PostDraw();
			}
		}
	}*/
	glEnable(GL_CULL_FACE);
}

void CLayout::SetFont(const char* font)
{
	this->Font = TTF_OpenFont(font, 32);
	if (!this->Font)
	{
		CLog::MyLog(LogType::Error, "Failed to load font %s",font);
		return;
	}
}

TTF_Font* CLayout::GetFont()
{
	return this->Font;
}

void CLayout::AddItem(int id, const char* name, glm::vec2 pos, glm::vec2 size)
{
	std::map<int, std::vector<std::shared_ptr<CObject2D>>>::iterator it;
	if (id == Object2DType::OBJECT2D_LABEL)	//Clabel
	{
		std::shared_ptr<CLabel> temp = std::make_shared<CLabel>();
		temp->SetPosition(pos);
		temp->SetSize(size);
		temp->SetName(name);
		temp->SetID(Object2DType::OBJECT2D_LABEL);
		
		Objects2D.push_back(temp);
		/*it = Objects2D.find(Object2DType::OBJECT2D_LABEL);
		if (it != Objects2D.end())
		{
			it->second.push_back(temp);
		}
		else
		{

			Objects2D.emplace(Object2DType::OBJECT2D_LABEL,std::vector<std::shared_ptr<CObject2D>>(1,temp));
		}*/
	}
	else if (id == Object2DType::OBJECT2D_IMAGE)	//CImage
	{
		std::shared_ptr<CImage> temp = std::make_shared<CImage>();
		temp->SetPosition(pos);
		temp->SetSize(size);
		temp->SetName(name);
		temp->SetID(Object2DType::OBJECT2D_IMAGE);

		Objects2D.push_back(temp);
		/*it = Objects2D.find(Object2DType::OBJECT2D_IMAGE);
		if (it != Objects2D.end())
		{
			it->second.push_back(temp);
		}
		else
		{
			Objects2D.emplace(Object2DType::OBJECT2D_IMAGE, std::vector<std::shared_ptr<CObject2D>>(1, temp));
		}*/
	}
	else if (id == Object2DType::OBJECT2D_BUTTON)	//Button
	{
		char LabelName[64];
		std::shared_ptr<CButton> temp = std::make_shared<CButton>();
		temp->SetPosition(pos);
		temp->SetSize(size);
		temp->SetName(name);
		temp->SetID(Object2DType::OBJECT2D_BUTTON);

		std::shared_ptr<CLabel> ButtonLabel = std::make_shared<CLabel>();
		sprintf_s(LabelName, "%s%s", name, "_Label");
		ButtonLabel->SetName(LabelName);
		ButtonLabel->BindParrentObject(temp);
		ButtonLabel->MoveObjectLayerUp();
		ButtonLabel->SetPosition(glm::vec2(temp->GetSize().x / 4, temp->GetSize().y / 4));
		ButtonLabel->SetID(Object2DType::OBJECT2D_LABEL);
		Objects2D.push_back(temp);
		CLayout::AddButtonToList(temp);

		temp->SetLabel(ButtonLabel);
		Objects2D.push_back(ButtonLabel);
		/*it = Objects2D.find(Object2DType::OBJECT2D_BUTTON);
		if (it != Objects2D.end())
		{
			it->second.push_back(temp);
		}
		else
		{
			Objects2D.emplace(Object2DType::OBJECT2D_BUTTON, std::vector<std::shared_ptr<CObject2D>>(1, temp));
		}*/
	}
}

std::vector<std::shared_ptr<CObject2D>> CLayout::GetObjectByType(int type)
{
	std::vector<CObject2D> List;
	/*for (auto it = Objects2D.begin(); it != Objects2D.end(); ++it)
	{
		if (it->first == type)
		{
			return it->second;
		}
	}*/
	return {};
}

std::vector<std::shared_ptr<CObject2D>> CLayout::GetButtons()
{
	return this->Buttons;
}

void CLayout::AddButtonToList(std::shared_ptr<CObject2D> btn)
{
	this->Buttons.push_back(btn);
}

const char* CLayout::GetName()
{
	return this->Name;
}

void CLayout::SetName(const char* name)
{
	this->Name = name;
}

void CLayout::Tick(double delta)
{
	Event->GetMouseMotion(this->MousePosX, this->MousePosY);
	for (auto a : Objects2D)
	{
		a->Tick(delta);
	}
	/*for (auto it = Objects2D.begin(); it != Objects2D.end(); ++it)
	{
		for (auto o : it->second)
		{
			o->Tick(delta);
		}
	}*/
	auto buttons = this->GetButtons();
	for (auto b : buttons)
	{
		auto butt = std::dynamic_pointer_cast<CButton>(b);
		if (butt != nullptr)
		{
			butt->IsClicked(Event->GetMouseData());
		}
	}
}
