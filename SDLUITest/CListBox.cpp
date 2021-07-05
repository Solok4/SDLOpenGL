#include "CListBox.h"
#include "COpengl.h"

CListBox::~CListBox()
{
	this->Label = nullptr;
}

void CListBox::CreateLabels()
{
	std::string LabelName = this->_Name + "_Label";
	this->Label = std::make_shared<CLabel>(LabelName.c_str(), glm::vec2(0.0f), glm::vec2(1.f), this->LayoutRef);
	this->Label->BindParrentObject(this);
	this->Label->MoveObjectLayerUp();
	this->Label->SetAligment(Object2DAligment::LEFT_TOP);
	this->Label->SetPadding(this->_Padding + vec2(5.f));
	this->Label->Prepare();
}

void CListBox::SetFont(TTF_Font* fon)
{
	this->FontSize = TTF_FontHeight(fon);
	this->Label->SetFont(fon);
}

void CListBox::SetText(std::string val)
{
	this->Value = val;
	if (!this->Label)
	{
		return;
	}
	this->Label->SetTextWrapped((int)this->_Size.x - ((int)this->Label->GetPadding().x * 2), this->Value.c_str());
	this->Label->SetPosition(glm::vec2(this->Label->GetRelativePosition().x, this->Label->GetRelativePosition().y + this->Label->GetSize().y));
}

void CListBox::PostDraw()
{
	if (this->_IsVisible)
	{
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		this->Label->PreDraw();
		OpenGL->SetModelMatrix(this->Label->GetModelMatrix());
		OpenGL->SetColorMaskLayout(this->Label->GetColorMask());
		this->Label->Draw();
		this->Label->PostDraw();
	}
}