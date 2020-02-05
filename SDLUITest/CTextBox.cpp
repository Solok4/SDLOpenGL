#include "CTextBox.h"
#include "CLayout.h"


//CTextBox::CTextBox()
//{
//}


CTextBox::CTextBox(const char* name, glm::vec2 position, glm::vec2 objsize, CLayout* ref) :CButton(name, position, objsize, ref)
{
	this->_ID = Object2DType::OBJECT2D_TEXTBOX;
	this->Value = std::string("");
	this->_Rotation = glm::vec3(180.f, 0.0f, 0.0f);
	CObject2D::Prepare();
	glGenTextures(1, &this->TextureID);
}

CTextBox::~CTextBox()
{
}

std::string CTextBox::GetValue()
{
	return this->Value;
}

void CTextBox::SetValue(std::string value)
{
	this->Value = value;
	auto label = this->GetLabel();
	if (!label)
	{
		return;
	}
	int labelWidth = (int)this->Label->GetSize().x;
	if (this->_Size.x >= (this->Label->GetSize().x+(this->GetPadding().x)*2))
	{
		if (this->Value.length() > 0)
		{
			label->SetText(value.c_str());
			this->Label->SetVisibility(true);
		}
		else
		{
			this->Label->SetVisibility(false);
		}
	}
	else
	{
		static int MaxLength = 0;
		if (MaxLength == 0)
		{
			MaxLength = this->Value.length();
		}
		if (MaxLength > this->Value.length())
		{
			label->SetText(value.c_str());
		}
	}
}

void CTextBox::Tick(double delta)
{
	if (this->IsLastHit)
	{
		this->LayoutRef->SetUsedTextBox(this);
	}
	else
	{
		this->LayoutRef->SetUsedTextBox(nullptr);
	}
}
