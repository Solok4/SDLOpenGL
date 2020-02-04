#pragma once
#include "CButton.h"
class CTextBox :
	public CButton
{
public:
	CTextBox()=delete;
	CTextBox(const char* name, /*CLayout* ref,*/ glm::vec2 position, glm::vec2 objsize) :CButton(name, /*ref,*/ position, objsize) { this->_ID = Object2DType::OBJECT2D_BUTTON; };
	~CTextBox();


private:
};

