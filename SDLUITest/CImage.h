#pragma once
#include "CObject2D.h"
class CImage :
	public CObject2D
{
public:
	CImage()=delete;
	CImage(const char* name, /*CLayout* ref,*/ glm::vec2 position, glm::vec2 objsize) :CObject2D(name, /*ref,*/ position, objsize) { this->_ID = Object2DType::OBJECT2D_IMAGE; };
	~CImage();

	//Bind texture to the object.
	void BindTexture(GLuint Tex);

	//Prepares object to draw.
	virtual void PreDraw() override;
	//Draws the object. Should be used after PreDraw func.
	virtual void Draw() override;
	//Cleans up things after drawing.
	virtual void PostDraw() override;
protected:

	GLuint TextureID;
};

