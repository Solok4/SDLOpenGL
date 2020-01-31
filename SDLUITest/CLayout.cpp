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
	std::map<int, std::vector<std::shared_ptr<CObject2D>>>::iterator it;
	if (id == Object2DType::OBJECT2D_LABEL)	//Clabel
	{
		std::shared_ptr<CLabel> temp = std::make_shared<CLabel>();
		temp->SetPosition(pos);
		temp->SetSize(size);
		temp->SetName(name);
		temp->SetID(Object2DType::OBJECT2D_LABEL);
		
		Objects2D.push_back(temp);
	}
	else if (id == Object2DType::OBJECT2D_IMAGE)	//CImage
	{
		std::shared_ptr<CImage> temp = std::make_shared<CImage>();
		temp->SetPosition(pos);
		temp->SetSize(size);
		temp->SetName(name);
		temp->SetID(Object2DType::OBJECT2D_IMAGE);

		Objects2D.push_back(temp);
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
#ifndef __EMSCRIPTEN__
		sprintf_s(LabelName, "%s%s", name, "_Label");
#else
		sprintf(LabelName, "%s%s", name, "_Label");
#endif
		ButtonLabel->SetName(LabelName);
		ButtonLabel->BindParrentObject(temp);
		ButtonLabel->MoveObjectLayerUp();
		ButtonLabel->SetAligment(Object2DAligment::CENTER);
		ButtonLabel->SetPosition(glm::vec2(temp->GetSize().x / 2, temp->GetSize().y / 2));
		ButtonLabel->SetID(Object2DType::OBJECT2D_LABEL);
		Objects2D.push_back(temp);
		CLayout::AddButtonToList(temp);

		temp->SetLabel(ButtonLabel);
		Objects2D.push_back(ButtonLabel);
	}
	else if (id == Object2DType::OBJECT2D_CONTAINER)	//CContainer
	{
		std::shared_ptr<CContainer> temp = std::make_shared<CContainer>();
		temp->SetPosition(pos);
		temp->SetSize(size);
		temp->SetName(name);
		temp->SetID(Object2DType::OBJECT2D_CONTAINER);

		Objects2D.push_back(temp);
	}
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
