#include "pch.h"
#include "CStaticMeshComponent.h"
#include "GL/glew.h"
#include <string>


CStaticMeshComponent::CStaticMeshComponent()
{
}


CStaticMeshComponent::~CStaticMeshComponent()
{
}

void CStaticMeshComponent::BindModel(std::shared_ptr<Model> model)
{
	this->_Model = model;
}

void CStaticMeshComponent::Draw()
{
	if (this->_Model != nullptr)
	{
		glBindVertexArray(this->_Model->VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D,)
		glDrawElements(GL_STATIC_DRAW, this->_Model->IndicesCount, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}


