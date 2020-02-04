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
	glEnable(GL_CULL_FACE);
}

void CLayout::AddItem(int id, const char* name, glm::vec2 pos, glm::vec2 size)
{
	if (id == Object2DType::OBJECT2D_LABEL)	//Clabel
	{
		std::shared_ptr<CLabel> temp = std::make_shared<CLabel>(name,pos,size);
		Objects2D.push_back(temp);
	}
	else if (id == Object2DType::OBJECT2D_IMAGE)	//CImage
	{
		std::shared_ptr<CImage> temp = std::make_shared<CImage>(name, pos, size);
		Objects2D.push_back(temp);
	}
	else if (id == Object2DType::OBJECT2D_BUTTON)	//Button
	{
		char LabelName[64];
		std::shared_ptr<CButton> temp = std::make_shared<CButton>(name, pos, size);

#ifndef __EMSCRIPTEN__
		sprintf_s(LabelName, "%s%s", name, "_Label");
#else
		sprintf(LabelName, "%s%s", name, "_Label");
#endif
		std::shared_ptr<CLabel> ButtonLabel = std::make_shared<CLabel>(LabelName, glm::vec2(temp->GetSize().x / 2, temp->GetSize().y / 2),glm::vec2(1.f));
		ButtonLabel->BindParrentObject(temp);
		ButtonLabel->MoveObjectLayerUp();
		ButtonLabel->SetAligment(Object2DAligment::CENTER);
		Objects2D.push_back(temp);
		CLayout::AddButtonToList(temp);

		temp->SetLabel(ButtonLabel);
		Objects2D.push_back(ButtonLabel);
	}
	else if (id == Object2DType::OBJECT2D_CONTAINER)	//CContainer
	{
		std::shared_ptr<CContainer> temp = std::make_shared<CContainer>(name,pos,size);
		Objects2D.push_back(temp);
	}
	else if (id == Object2DType::OBJECT2D_TEXTBOX)
	{
		std::shared_ptr<CTextBox> temp = std::make_shared<CTextBox>(name, pos, size);
		Objects2D.push_back(temp);
		CLayout::AddButtonToList(temp);
	}
}

std::vector<std::shared_ptr<CObject2D>> CLayout::GetButtons()
{
	return this->Buttons;
}

void CLayout::AddButtonToList(std::shared_ptr<CObject2D> btn)
{
	auto temp = std::dynamic_pointer_cast<CButton>(btn);
	if (temp != nullptr)
	{
		this->Buttons.push_back(btn);
		return;
	}
	temp = std::dynamic_pointer_cast<CTextBox>(btn);
	if (temp != nullptr)
	{
		this->Buttons.push_back(btn);
		return;
	}
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
