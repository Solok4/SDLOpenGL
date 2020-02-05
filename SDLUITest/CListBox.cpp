#include "CListBox.h"
#include "COpengl.h"



void CListBox::CreateLabels(int rows)
{
	for (int i = this->LabelVector.size(); i <= rows-1; i++)
	{
		auto Label = std::make_shared<CLabel>(std::to_string(i).c_str(), glm::vec2(this->GetSize().x / 2,this->_Padding.y), glm::vec2(1.f), this->LayoutRef);
		Label->Prepare();
		Label->BindParrentObject(this);
		Label->MoveObjectLayerUp();
		Label->SetAligment(Object2DAligment::LEFT_TOP);
		Label->SetPadding(glm::vec2(5.0f));
		this->LabelVector.push_back(Label);
	}
}

void CListBox::SetFont(TTF_Font* fon)
{
	this->LBoxFont = fon;
	this->FontSize = TTF_FontHeight(fon);
	this->LettersInARow = (int)floor((int)this->_Size.x / this->FontSize)*2.5;
	for (auto a : this->LabelVector)
	{
		a->SetFont(fon);
	}
}

void CListBox::SetText(std::string val)
{
	this->Value = val;
	int NextHeight = 20;
	int SubstrBegin = 0;
	std::string tempString = val;
	int nextSpace = 0;
	int CombinedSpaces = 0;
	for (int i = 0; i <= this->LabelVector.size()-1; i++)
	{
		if (!this->LabelVector[i])
		{
			return;
		}
		if (SubstrBegin == this->Value.length())
		{
			break;
		}
		CombinedSpaces = 0;
		this->LabelVector[i]->SetPosition(glm::vec2(this->_Padding.x, NextHeight));

		//int SubstrBegin = this->LettersInARow * i;
		for (;;) 
		{
			nextSpace = tempString.find_first_of(" ");
			if (nextSpace == -1)
			{
				CombinedSpaces += tempString.length();
				break;
			}
			else
			{
				CombinedSpaces += nextSpace+1;
				tempString = tempString.substr(nextSpace + 1);
			}
			
			if (CombinedSpaces > (int)(this->LettersInARow * .85) && CombinedSpaces < this->LettersInARow)
			{
				break;
			}
		}

		this->LabelVector[i]->SetText(this->Value.substr(SubstrBegin, CombinedSpaces).c_str());
		SubstrBegin += CombinedSpaces;
		NextHeight +=  (int)this->LabelVector[i]->GetSize().y+ (int)this->LabelVector[i]->GetPadding().y;
	}
}

void CListBox::PostDraw()
{
	if (this->_IsVisible)
	{
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		
		for (auto a : this->LabelVector)
		{
			a->PreDraw();
			OpenGL->SetModelMatrixLayout(a->GetModelMatrix());
			OpenGL->SetColorMaskLayout(a->GetColorMask());
			a->Draw();
			a->PostDraw();
		}
	}
}

CListBox::~CListBox()
{
}
