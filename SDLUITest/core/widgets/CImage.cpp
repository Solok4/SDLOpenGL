#include "CImage.h"

CImage::~CImage()
{
}

void CImage::PreDraw()
{
	if (this->_IsVisible)
	{
		glBindVertexArray(this->_VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glActiveTexture(GL_TEXTURE0);
	}
}

void CImage::Draw()
{
	if (this->_IsVisible)
	{
		glBindTexture(GL_TEXTURE_2D, this->TextureID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void CImage::PostDraw()
{
	if (this->_IsVisible)
	{
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}

void CImage::BindTexture(GLuint Tex)
{
	this->TextureID = Tex;
}

void CImage::Prepare()
{
	CObject2D::Prepare();
}