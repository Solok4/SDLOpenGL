#include "pch.h"
#include "CLayout.h"


CLayout::CLayout()
{

}


CLayout::~CLayout()
{
}

void CLayout::SetWindowData(SDL_Window * WND)
{
	SDL_GetWindowPosition(WND, &WindowPosX, &WindowPosY);
	SDL_GetWindowSize(WND, &WindowWidth, &WindowHeight);
}

void CLayout::SetShaderProgram(GLuint* Program)
{
	this->ShaderProgram = Program;
}

void CLayout::PrepareToLoop()
{
	for (CObject2D* o : Objects2D)
	{
		o->Prepare();
		CButton* TryButton = dynamic_cast<CButton*>(o);
		if (!(TryButton == NULL))
		{
			TryButton->AttachFunc([]() {CLog::MyLog(0, "Print string"); });
			continue;
		}
		CLabel*TryLabel = dynamic_cast<CLabel*>(o);
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

void CLayout::Draw(COpengl* opengl)
{
	for (CObject2D* o : Objects2D)
	{
		opengl->SetModelMatrix(o->GetModelMatrix());
		o->PreDraw();
		o->Draw();
		o->PostDraw();
		if (o->GetID() == Object2DType::OBJECT2D_BUTTON)
		{
			CButton* temp = dynamic_cast<CButton*>(o);
			opengl->SetModelMatrix(temp->GetLabel()->GetModelMatrix());
			temp->GetLabel()->PreDraw();
			temp->GetLabel()->Draw();
			temp->GetLabel()->PostDraw();
		}
	}
}

void CLayout::SetFont(const char* font)
{
	this->Font = TTF_OpenFont(font, 32);
	if (!this->Font)
	{
		CLog::MyLog(1, "Failed to load font " + std::string(font));
	}
}

TTF_Font* CLayout::GetFont()
{
	return this->Font;
}

void CLayout::AddItem(int id,std::string name, glm::vec2 pos, glm::vec2 size)
{
	if (id == Object2DType::OBJECT2D_LABEL)	//Clabel
	{
		CLabel* temp = new CLabel();
		temp->SetPosition(pos);
		temp->SetSize(size);
		temp->SetName(name);
		temp->SetID(Object2DType::OBJECT2D_LABEL);
		Objects2D.push_back(temp);
	}
	else if (id == Object2DType::OBJECT2D_IMAGE)	//CImage
	{
		CImage* temp = new CImage();
		temp->SetPosition(pos);
		temp->SetSize(size);
		temp->SetName(name);
		temp->SetID(Object2DType::OBJECT2D_IMAGE);
		Objects2D.push_back(temp);
	}
	else if (id == Object2DType::OBJECT2D_BUTTON)
	{
		CButton* temp = new CButton();
		temp->SetPosition(pos);
		temp->SetSize(size);
		temp->SetName(name);
		temp->SetID(Object2DType::OBJECT2D_BUTTON);
		Objects2D.push_back(temp);
	}


}

CObject2D * CLayout::FindObjectByName(std::string name)
{
	for (CObject2D* o : Objects2D)
	{
		if (o->GetName() == name)
		{
			return o;
		}
	}
	return NULL;
}

std::vector<CObject2D*> CLayout::GetObjectByType(int type)
{
	std::vector<CObject2D*> List;
	for (CObject2D* o : Objects2D)
	{
		if (o->GetID() == type)
		{
			List.push_back(o);
		}
	}
	return List;
}

void CLayout::GetMousePosition(int x, int y)
{
	MousePosX = x;
	MousePosY = y;
}
