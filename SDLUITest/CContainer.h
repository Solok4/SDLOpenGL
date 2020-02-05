#pragma once
#include "CObject2D.h"
class CContainer :
	public CObject2D
{
public:
	CContainer()=delete;
	CContainer(const char* name, glm::vec2 position, glm::vec2 objsize,CLayout* ref) :CObject2D(name, position, objsize,ref) { this->_ID = Object2DType::OBJECT2D_CONTAINER; }
	~CContainer();
};

