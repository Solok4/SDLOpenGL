#include "CTextBox.h"
#include "../../CLayout.h"

#define DEAD_SPACE 20

CTextBox::CTextBox(const char* name, glm::vec2 position, glm::vec2 objsize, CLayout* ref) :CButton(name, position, objsize, ref)
{
	this->_ID = Object2DType::OBJECT2D_TEXTBOX;
	this->Value = std::string("");
	CObject2D::Prepare();
	this->CanEdit = true;
	this->EnterFunc = []() {};
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
	if (this->CanEdit)
	{
		this->Value = value;
		auto label = this->GetLabel();
		if (!label)
		{
			return;
		}
		int labelWidth = (int)this->Label->GetSize().x;
		static int MaxLength = 0;
		if ((this->_Size.x >= (this->Label->GetSize().x + (this->GetPadding().x) * 2 + DEAD_SPACE))
			&& ((MaxLength == 0) || (MaxLength > this->Value.length())))
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
			if (MaxLength == 0)
			{
				MaxLength = this->Value.length();
			}
			if (MaxLength > this->Value.length())
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
				label->SetText(value.substr(this->Value.length() - MaxLength, MaxLength).c_str());
			}
		}
	}
}

void CTextBox::BindEnterFunc(std::function<void()> func)
{
	this->EnterFunc = func;
}

void CTextBox::OnEnter()
{
	this->EnterFunc();
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