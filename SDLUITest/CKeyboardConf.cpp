
#include "pch.h"
#include "CKeyboardConf.h"
#include "CLog.h"

std::unique_ptr<CKeyboardConf> KeyboardConf;

CKeyboardConf::CKeyboardConf()
{
	for (int i = 0 ; i < 322; i++)
	{
		this->KeyBtns[i].IsPressed = false;
		this->KeyBtns[i].IsTriggerType = false;
		this->KeyBtns[i].LastStatus = false;
		this->KeyResults[i] = false;
	}

}


CKeyboardConf::~CKeyboardConf()
{
}

void CKeyboardConf::ProcessButtons(std::array<bool, 322> ButArray)
{
	for (int i = 0; i < 322; i++)
	{
		this->KeyBtns[i].IsPressed = ButArray[i];
		if (this->KeyBtns[i].IsTriggerType)
		{
			if ((this->KeyBtns[i].LastStatus == false) && (this->KeyBtns[i].IsPressed == true))
			{
				this->KeyResults[i] = true;
				this->KeyBtns[i].LastStatus = true;
			}
			else if ((this->KeyBtns[i].LastStatus == true) && (this->KeyBtns[i].IsPressed == true))
			{
				this->KeyResults[i] = false;
			}
			else 
			{
				this->KeyResults[i] = false;
				this->KeyBtns[i].LastStatus = false;
			}
		}
		else
		{
			this->KeyResults[i] = this->KeyBtns[i].IsPressed;
		}
	}
}

void CKeyboardConf::SetKeyTriggerStatus(unsigned int Key, bool Status)
{
	this->KeyBtns[Key].IsTriggerType = Status;
}

std::array<bool, 322> CKeyboardConf::GetKeyButtons()
{
	return this->KeyResults;
}
