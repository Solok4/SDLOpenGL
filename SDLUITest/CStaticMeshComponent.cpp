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
		if(this->_Model->HasTexcords)
			glEnableVertexAttribArray(1);
		if (this->_Model->HasNormals)
			glEnableVertexAttribArray(2);
		glActiveTexture(GL_TEXTURE0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_Model->EBO);
		glBindTexture(GL_TEXTURE_2D, this->_Model->Tex);
		glDrawElements(GL_STATIC_DRAW, this->_Model->IndicesCount, GL_UNSIGNED_INT, 0);
		if (this->_Model->HasNormals)
			glDisableVertexAttribArray(2);
		if (this->_Model->HasTexcords)
			glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}


