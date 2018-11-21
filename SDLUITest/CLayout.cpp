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
		o->SetTexture("Assets/Textures/TestTex.jpg");
	}
}

void CLayout::Draw(COpengl* opengl)
{
	for (CObject2D* o : Objects2D)
	{
		opengl->SetModelMatrix(o->GetModelMatrix());
		o->Draw();
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

void CLayout::AddItem(int id, glm::vec2 pos, glm::vec2 size)
{
	if (id == 0)	//CObject2D
	{
		CObject2D* temp = new CObject2D();
		temp->SetPosition(pos);
		temp->SetSize(size);
		Objects2D.push_back(temp);
	}
	else if (id == 1)	//Clabel
	{
		CLabel* temp = new CLabel(1);
		temp->SetPosition(pos);
		temp->SetSize(size);
		Objects2D.push_back(temp);
	}


}

void CLayout::GetMousePosition(int x, int y)
{
	MousePosX = x;
	MousePosY = y;
}
