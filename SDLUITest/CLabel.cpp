#include "pch.h"
#include "CLabel.h"
#include "gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"


CLabel::CLabel(unsigned int ID)
{
	this->_ID = ID;
	this->_Size = glm::vec2(20.0f, 10.0f);
}


CLabel::~CLabel()
{
}

