#include "CLayout.h"
#include <memory>
#include "core/renderer/COpengl.h"
#include "core/managers/CEventManager.h"

CLayout::CLayout()
{
	this->RefreshWindowData();
	this->UsedTestBox = nullptr;
	this->RefreshWindowData();
	this->RootContainer = std::make_shared<CContainer>("root", vec2(0, 0), vec2(this->WNDInfo->ScreenWidth, this->WNDInfo->ScreenHeight), this);
	this->Objects2D.push_back(this->RootContainer);
}

CLayout::~CLayout()
{
	CLog::debug("LayoutDestructor %s", this->Name);
}

void CLayout::RefreshWindowData()
{
	this->WNDInfo = WindowManager->GetWindowInfo();
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
		OpenGL->SetModelMatrix(a->GetModelMatrix());
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
		std::shared_ptr<CLabel> temp = std::make_shared<CLabel>(name, pos, size, this);
		temp->BindParrentObject(&*this->RootContainer);
		Objects2D.push_back(temp);
	}
	else if (id == Object2DType::OBJECT2D_IMAGE)	//CImage
	{
		std::shared_ptr<CImage> temp = std::make_shared<CImage>(name, pos, size, this);
		temp->BindParrentObject(&*this->RootContainer);
		Objects2D.push_back(temp);
	}
	else if (id == Object2DType::OBJECT2D_BUTTON)	//Button
	{
		std::shared_ptr<CButton> temp = std::make_shared<CButton>(name, pos, size, this);
		temp->BindParrentObject(&*this->RootContainer);
		Objects2D.push_back(temp);
		CLayout::AddButtonToList(temp);
	}
	else if (id == Object2DType::OBJECT2D_CONTAINER)	//CContainer
	{
		std::shared_ptr<CContainer> temp = std::make_shared<CContainer>(name, pos, size, this);
		temp->BindParrentObject(&*this->RootContainer);
		Objects2D.push_back(temp);
	}
	else if (id == Object2DType::OBJECT2D_TEXTBOX)		//CTextBox
	{
		std::shared_ptr<CTextBox> temp = std::make_shared<CTextBox>(name, pos, size, this);
		temp->BindParrentObject(&*this->RootContainer);
		Objects2D.push_back(temp);
		CLayout::AddButtonToList(temp);
	}
	else if (id == Object2DType::OBJECT2D_LISTBOX)
	{
		std::shared_ptr<CListBox> temp = std::make_shared<CListBox>(name, pos, size, this);
		temp->BindParrentObject(&*this->RootContainer);
		Objects2D.push_back(temp);
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
	EventManager->GetMouseMotion(this->MousePosX, this->MousePosY);
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
			butt->IsClicked(EventManager->GetMouseData());
		}
	}
	this->ProcessTextEditing();
}

void CLayout::ProcessTextEditing()
{
	if (this->UsedTestBox == nullptr)
	{
		return;
	}
	if (this->LayoutIsEditing)
	{
		bool bChanged = false;
		uint32_t SingleKey = EventManager->GetRawKey();
		std::string Text = this->UsedTestBox->GetValue();
		if (SingleKey == SDLK_BACKSPACE && Text.length() > 0)
		{
			Text.pop_back();
			bChanged = true;
		}
		else if (SingleKey == SDLK_c && SDL_GetModState() & KMOD_CTRL)
		{
			SDL_SetClipboardText(Text.c_str());
		}
		else if (SingleKey == SDLK_v && SDL_GetModState() & KMOD_CTRL)
		{
			Text = SDL_GetClipboardText();
		}
		else if (SingleKey == SDLK_RETURN)
		{
			this->UsedTestBox->OnEnter();
			Text.clear();
			bChanged = true;
		}

		if (EventManager->GetIsEditing())
		{
			if (SDL_GetModState() & KMOD_CAPS)
			{
				SingleKey -= 32;
			}
			if (SDL_GetModState() & KMOD_SHIFT)
			{
				if (SingleKey >= 97 && SingleKey <= 122)
				{
					SingleKey -= 32;
				}
				else if (SingleKey >= 65 && SingleKey <= 90)
				{
					SingleKey += 32;
				}
			}

			Text.push_back(SingleKey);
			bChanged = true;
		}
		if (bChanged)
		{
			this->UsedTestBox->SetValue(Text);
		}
	}
}

void CLayout::SetUsedTextBox(CTextBox* tbox)
{
	if (tbox != nullptr)
	{
		SDL_StartTextInput();
		this->UsedTestBox = tbox;
		this->LayoutIsEditing = true;
	}
	else
	{
		SDL_StopTextInput();
		this->UsedTestBox = nullptr;
		this->LayoutIsEditing = false;
	}
}

void CLayout::RefreshScreenSize()
{
	this->RootContainer->SetSize(vec2(this->WNDInfo->ScreenWidth, this->WNDInfo->ScreenHeight));
}