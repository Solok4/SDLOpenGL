#pragma once
#include "CObject2D.h"
class CContainer :
	public CObject2D
{
public:
	CContainer()=delete;
	CContainer(const char* name, /*CLayout* ref,*/ glm::vec2 position, glm::vec2 objsize) :CObject2D(name, /*ref,*/ position, objsize) { this->_ID = Object2DType::OBJECT2D_CONTAINER; }
	~CContainer();
};

