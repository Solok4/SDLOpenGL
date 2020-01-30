#include "CVar.h"


void CVar::SetCurrentValue(int value)
{
	this->_CurrentValue->IntValue = value;
	if (this->_ChangeValueFunc != nullptr)
	{
		this->_ChangeValueFunc(*this);
	}
}

void CVar::SetCurrentValue(double value)
{
	this->_CurrentValue->DoubleValue = value;
	if (this->_ChangeValueFunc != nullptr)
	{
		this->_ChangeValueFunc(*this);
	}
}

void CVar::SetCurrentValue(std::string value)
{
	this->_CurrentValue->StringValue = (char*)value.c_str();
	if (this->_ChangeValueFunc != nullptr)
	{
		this->_ChangeValueFunc(*this);
	}
}

CVar::~CVar()
{
}
