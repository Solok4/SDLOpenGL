#include "pch.h"
#include "CLayout.h"
#include <memory>


CLayout::CLayout()
{

}


CLayout::~CLayout()
{
	CLog::MyLog(0, "LayoutDestructor "+this->Name);
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
			auto TryButton = std::dynamic_pointer_cast<CButton>(o);
			if (!(TryButton == NULL))
			{
				TryButton->AttachFunc([]() {CLog::MyLog(0, "Print string"); });
				continue;
			}
			auto TryLabel = std::dynamic_pointer_cast<CLabel>(o);
			if (!(TryLabel == NULL))
			{
				TryLabel->SetFont(this->GetFont());
				TryLabel->SetText("Test");
				continue;
			}
			o->LoadTexture("Assets/Textures/TestTex.jpg");
			o->BindTexture(o->GetTexture());
		}
	}
}

void CLayout::Draw(COpengl* opengl)
{
	for (auto it = Objects2D.begin(); it != Objects2D.end(); ++it)
	{	
		for (auto o : it->second)
		{
			o->PreDraw();
			opengl->SetModelMatrix(o->GetModelMatrix());
			o->Draw();
			o->PostDraw();
			if (o->GetID() == Object2DType::OBJECT2D_BUTTON)
			{
				auto temp = std::dynamic_pointer_cast<CButton>(o);
				
				temp->GetLabel()->PreDraw();
				opengl->SetModelMatrix(temp->GetLabel()->GetModelMatrix());
				temp->GetLabel()->Draw();
				temp->GetLabel()->PostDraw();
			}
		}
	}
}

void CLayout::SetFont(const char* font)
{
	this->Font = TTF_OpenFont(font, 32);
	if (!this->Font)
	{
		CLog::MyLog(1, "Failed to load font " + std::string(font));
		return;
	}
}

TTF_Font* CLayout::GetFont()
{
	return this->Font;
}

void CLayout::AddItem(int id,std::string name, glm::vec2 pos, glm::vec2 size)
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

std::shared_ptr<CObject2D> CLayout::FindObjectByName(std::string name)
{
	for (auto it = Objects2D.begin(); it != Objects2D.end(); ++it)
	{
		for (auto o : it->second)
		{
			if (o->GetName() == name)
			{
				return o;
			}
		}
	}
	return NULL;
}

std::vector<std::shared_ptr<CObject2D>> CLayout::GetObjectByType(int type)
{
	std::vector<CObject2D*> List;
	for (auto it = Objects2D.begin(); it != Objects2D.end(); ++it)
	{
		if (it->first == type)
		{
			return it->second;
		}
	}
	return {};
}

std::string CLayout::GetName()
{
	return this->Name;
}

void CLayout::SetName(std::string name)
{
	this->Name = name;
}

void CLayout::GetMousePosition(int x, int y)
{
	this->MousePosX = x;
	this->MousePosY = y;
}
