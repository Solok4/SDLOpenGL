#include "pch.h"
#include "CLayout.h"
#include <memory>


CLayout::CLayout()
{

}


CLayout::~CLayout()
{
	CLog::MyLog(LogType::Log, "LayoutDestructor %s",this->Name);
}

void CLayout::SetWindowData(SDL_Window * WND)
{
	SDL_GetWindowPosition(WND, &WindowPosX, &WindowPosY);
	SDL_GetWindowSize(WND, &WindowWidth, &WindowHeight);
}

void CLayout::PrepareToLoop()
{
	for(auto it = Objects2D.begin();it != Objects2D.end();++it)
	{
		for (auto o : it->second)
		{
			o->Prepare();
		}
	}
}

void CLayout::Draw(COpengl* opengl)
{
	glDisable(GL_CULL_FACE);
	for (auto it = Objects2D.begin(); it != Objects2D.end(); ++it)
	{	
		for (auto o : it->second)
		{
			o->PreDraw();
			opengl->SetModelMatrixLayout(o->GetModelMatrix());
			opengl->SetColorMaskLayout(o->GetColorMask());
			o->Draw();
			o->PostDraw();
			if (o->GetID() == Object2DType::OBJECT2D_BUTTON)
			{
				auto temp = std::dynamic_pointer_cast<CButton>(o);
				
				temp->GetLabel()->PreDraw();
				opengl->SetModelMatrixLayout(temp->GetLabel()->GetModelMatrix());
				temp->GetLabel()->Draw();
				temp->GetLabel()->PostDraw();
			}
		}
	}
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
		std::shared_ptr<CLabel> temp(new CLabel);
		temp->SetPosition(pos);
		temp->SetSize(size);
		temp->SetName(name);
		temp->SetID(Object2DType::OBJECT2D_LABEL);
		
		it = Objects2D.find(Object2DType::OBJECT2D_LABEL);
		if (it != Objects2D.end())
		{
			it->second.push_back(temp);
		}
		else
		{

			Objects2D.emplace(Object2DType::OBJECT2D_LABEL,std::vector<std::shared_ptr<CObject2D>>(1,temp));
		}
	}
	else if (id == Object2DType::OBJECT2D_IMAGE)	//CImage
	{
		std::shared_ptr<CImage> temp(new CImage);
		temp->SetPosition(pos);
		temp->SetSize(size);
		temp->SetName(name);
		temp->SetID(Object2DType::OBJECT2D_IMAGE);
		it = Objects2D.find(Object2DType::OBJECT2D_IMAGE);
		if (it != Objects2D.end())
		{
			it->second.push_back(temp);
		}
		else
		{
			Objects2D.emplace(Object2DType::OBJECT2D_IMAGE, std::vector<std::shared_ptr<CObject2D>>(1, temp));
		}
	}
	else if (id == Object2DType::OBJECT2D_BUTTON)
	{
		std::shared_ptr<CButton> temp(new CButton);
		temp->SetPosition(pos);
		temp->SetSize(size);
		temp->SetName(name);
		temp->SetID(Object2DType::OBJECT2D_BUTTON);
		CLayout::AddButtonToList(temp);
		it = Objects2D.find(Object2DType::OBJECT2D_BUTTON);
		if (it != Objects2D.end())
		{
			it->second.push_back(temp);
		}
		else
		{
			Objects2D.emplace(Object2DType::OBJECT2D_BUTTON, std::vector<std::shared_ptr<CObject2D>>(1, temp));
		}
		//it = Objects2D.find(Object2DType::OBJECT2D_LABEL);
		//if (it != Objects2D.end())
		//{
		//	it->second.push_back(temp->Label);
		//}
		//else
		//{
		//	Objects2D.emplace(Object2DType::OBJECT2D_LABEL, std::vector<CObject2D*>(1, temp->Label));
		//}
	}


}

std::vector<std::shared_ptr<CObject2D>> CLayout::GetObjectByType(int type)
{
	std::vector<CObject2D> List;
	for (auto it = Objects2D.begin(); it != Objects2D.end(); ++it)
	{
		if (it->first == type)
		{
			return it->second;
		}
	}
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

void CLayout::GetMousePosition(int x, int y)
{
	this->MousePosX = x;
	this->MousePosY = y;
}

void CLayout::Tick(uint32_t delta)
{
	for (auto it = Objects2D.begin(); it != Objects2D.end(); ++it)
	{
		for (auto o : it->second)
		{
			o->Tick(delta);
		}
	}
}
